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
#include "UI/SGameResultWidget_Multi.h"
#include "Components/TextBlock.h"

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

void ASPlayerController_Multi::ShowWinnerUI_Implementation()
{
	if (HasAuthority() == false)
	{
		if (IsValid(WinnerUIClass) == true)
		{
			USGameResultWidget_Multi* WinnerUI = CreateWidget<USGameResultWidget_Multi>(this, WinnerUIClass);
			if (IsValid(WinnerUI) == true)
			{
				WinnerUI->AddToViewport(3);
				WinnerUI->RankingText->SetText(FText::FromString(TEXT("Rank 1")));

				FInputModeUIOnly Mode;
				Mode.SetWidgetToFocus(WinnerUI->GetCachedWidget());
				SetInputMode(Mode);

				bShowMouseCursor = true;
			}
		}
    }
}

void ASPlayerController_Multi::ShowLoserUI_Implementation(int32 InRanking)
{
    if (HasAuthority() == false)
    {
        if (IsValid(LoserUIClass) == true)
        {
            USGameResultWidget_Multi* LoserUI = CreateWidget<USGameResultWidget_Multi>(this, LoserUIClass);
            if (IsValid(LoserUI) == true)
            {
                LoserUI->AddToViewport(3);
                FString RankingString = FString::Printf(TEXT("Rank %d"), InRanking);
                LoserUI->RankingText->SetText(FText::FromString(RankingString));

                FInputModeUIOnly Mode;
                Mode.SetWidgetToFocus(LoserUI->GetCachedWidget());
                SetInputMode(Mode);

                bShowMouseCursor = true;
            }
        }
    }
}

void ASPlayerController_Multi::ReturnToTitle_Implementation()
{
    // ������ �ƴ� Ŭ���̾�Ʈ �������� ���� �̵�
    if (HasAuthority() == false)
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("LoadingLevel")), true, FString(TEXT("NextLevel=TitleLevel")));
    }
}

void ASPlayerController_Multi::BeginPlay()
{
    Super::BeginPlay();

    // 'PlayerState' Replication ��ٸ��� ���� 1�� ������
    FTimerHandle replicationTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(replicationTimerHandle, FTimerDelegate::CreateLambda([&]()
    {
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

        // �˸�â ���� ����
        if (IsValid(NotificationWidgetClass) == true)
        {
            UUserWidget* NotificationWidget = CreateWidget<UUserWidget>(this, NotificationWidgetClass);
            if (IsValid(NotificationWidget) == true)
            {
                NotificationWidget->AddToViewport(1);
                NotificationWidget->SetVisibility(ESlateVisibility::Visible);
            }
        }

        // 'OnCurrentKillCountReachedMaxDelegate' ��������Ʈ�� ����Լ� ���ε�
        ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
        if (IsValid(SPlayerState) == true)
        {
            SPlayerState->OnCurrentKillCountReachedMaxDelegate.AddDynamic(this, &ThisClass::OnCurrentKillCountReachedMax);
        }
    }), 0.5f, false);
}

void ASPlayerController_Multi::OnCurrentKillCountReachedMax()
{
    // ������ �÷��̾� ��� �й�
    ASGameMode_Multi* GameMode = Cast<ASGameMode_Multi>(UGameplayStatics::GetGameMode(this));
    if (HasAuthority() == true && IsValid(GameMode) == true)
    {
        for (auto AlivePlayerController : GameMode->GetAlivePlayerControllers()) {
            if (IsValid(AlivePlayerController) == true && AlivePlayerController != this) {
                GameMode->OnControllerDead(AlivePlayerController);
            }
        }
    }
}
