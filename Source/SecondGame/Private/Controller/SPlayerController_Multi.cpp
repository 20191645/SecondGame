// SPlayerController_Multi.cpp

#include "Controller/SPlayerController_Multi.h"
#include "Blueprint/UserWidget.h"
#include "UI/SHUD.h"
#include "Game/SPlayerState.h"
#include "Component/SStatComponent.h"
#include "Character/SCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Game/SGameMode_Multi.h"

void ASPlayerController_Multi::ToggleManualWidget()
{
    checkf(IsValid(ManualWidgetInstance) == true, TEXT("Invalid MaualWidgetInstance"));

    // ���۹� ���� �ѱ�
    if (false == bIsManualOn)
    {
        ManualWidgetInstance->SetVisibility(ESlateVisibility::Visible);
    }
    // ���۹� ���� ����
    else
    {
        ManualWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
    }

    bIsManualOn = !bIsManualOn;
}

void ASPlayerController_Multi::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, NotificationText);
}

void ASPlayerController_Multi::OnOwningCharacterDead()
{
    ASGameMode_Multi* GameMode = Cast<ASGameMode_Multi>(UGameplayStatics::GetGameMode(this));
    if (HasAuthority() == true && IsValid(GameMode) == true)
    {
        GameMode->OnControllerDead(this);
    }
}

void ASPlayerController_Multi::BeginPlay()
{
    Super::BeginPlay();

    // SetInputMode(): �÷��� ��ư ������ �ٷ� ��Ŀ�� �ǰԲ� ��
    FInputModeGameOnly InputModeGameOnly;
    SetInputMode(InputModeGameOnly);

    // ������ ���� ȭ�鿡 �߰�
    if (IsValid(CrosshairUIClass) == true)
    {
        UUserWidget* CrosshairUIInstance = CreateWidget<UUserWidget>(this, CrosshairUIClass);
        if (IsValid(CrosshairUIInstance) == true)
        {
            CrosshairUIInstance->AddToViewport();
            CrosshairUIInstance->SetVisibility(ESlateVisibility::Visible);
        }
    }

    // HUD ���� ȭ�鿡 �߰�
    if (IsValid(HUDWidgetClass) == true)
    {
        HUDWidget = CreateWidget<USHUD>(this, HUDWidgetClass);
        if (IsValid(HUDWidget) == true)
        {
            HUDWidget->AddToViewport();

            // 'PlayerState' ���ε�
            ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
            if (IsValid(SPlayerState) == true)
            {
                HUDWidget->BindPlayerState(SPlayerState);
            }

            // 'StatComponent' ���ε�
            ASCharacter* PC = GetPawn<ASCharacter>();
            if (IsValid(PC) == true)
            {
                USStatComponent* StatComponent = PC->GetStatComponent();
                if (IsValid(StatComponent) == true)
                {
                    HUDWidget->BindStatComponent(StatComponent);
                }
            }

            // 'WeaponSlot' �ʱ�ȭ
            HUDWidget->SetWeaponSlot();
        }
    }

    // ���۹� ���� ��ü ����
    if (IsValid(ManualWidgetClass) == true)
    {
        ManualWidgetInstance = CreateWidget<UUserWidget>(this, ManualWidgetClass);
        if (IsValid(ManualWidgetInstance) == true)
        {
            ManualWidgetInstance->AddToViewport(3);
            ManualWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    if (IsValid(NotificationWidgetClass) == true)
    {
        UUserWidget* NotificationWidget = CreateWidget<UUserWidget>(this, NotificationWidgetClass);
        if (IsValid(NotificationWidget) == true)
        {
            NotificationWidget->AddToViewport(1);
            NotificationWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
}