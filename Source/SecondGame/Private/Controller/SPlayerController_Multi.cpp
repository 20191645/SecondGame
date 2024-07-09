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

    // SetInputMode(): 플레이 버튼 누르면 바로 포커싱 되게끔 함
    FInputModeGameOnly InputModeGameOnly;
    SetInputMode(InputModeGameOnly);

    // 조준점 위젯 화면에 추가
    if (IsValid(CrosshairUIClass) == true)
    {
        UUserWidget* CrosshairUIInstance = CreateWidget<UUserWidget>(this, CrosshairUIClass);
        if (IsValid(CrosshairUIInstance) == true)
        {
            CrosshairUIInstance->AddToViewport();
            CrosshairUIInstance->SetVisibility(ESlateVisibility::Visible);
        }
    }

    // HUD 위젯 화면에 추가
    if (IsValid(HUDWidgetClass) == true)
    {
        HUDWidget = CreateWidget<USHUD>(this, HUDWidgetClass);
        if (IsValid(HUDWidget) == true)
        {
            HUDWidget->AddToViewport();

            // 'PlayerState' 바인드
            ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
            if (IsValid(SPlayerState) == true)
            {
                HUDWidget->BindPlayerState(SPlayerState);
            }

            // 'StatComponent' 바인드
            ASCharacter* PC = GetPawn<ASCharacter>();
            if (IsValid(PC) == true)
            {
                USStatComponent* StatComponent = PC->GetStatComponent();
                if (IsValid(StatComponent) == true)
                {
                    HUDWidget->BindStatComponent(StatComponent);
                }
            }

            // 'WeaponSlot' 초기화
            HUDWidget->SetWeaponSlot();
        }
    }
}