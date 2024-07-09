// SPlayerController_Multi.cpp

#include "Controller/SPlayerController_Multi.h"
#include "Blueprint/UserWidget.h"
#include "UI/SHUD.h"
#include "Game/SPlayerState.h"
#include "Component/SStatComponent.h"
#include "Character/SCharacter.h"

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
}