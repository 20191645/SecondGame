// SPlayerController.cpp

#include "Controller/SPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/SHUD.h"
#include "Game/SPlayerState.h"
#include "Component/SStatComponent.h"
#include "Character/SCharacter.h"
#include "UI/SGameResultWidget_Single.h"

void ASPlayerController::BeginPlay()
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

    // 'OnCurrentKillCountReachedMaxDelegate, OnCurrentDeathCountReachedMaxDelegate' 델리게이트에 멤버함수 바인드
    ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
    if (IsValid(SPlayerState) == true)
    {
        SPlayerState->OnCurrentKillCountReachedMaxDelegate.AddDynamic(this, &ThisClass::OnCurrentKillCountReachedMax);
        SPlayerState->OnCurrentDeathCountReachedMaxDelegate.AddDynamic(this, &ThisClass::OnCurrentDeathCountReachedMax);
    }
}

void ASPlayerController::OnCurrentKillCountReachedMax()
{
    // 2초 딜레이 후 게임 클리어 위젯 화면에 추가
    FTimerHandle gameTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(gameTimerHandle, FTimerDelegate::CreateLambda([&]()
    {
        if (IsValid(GameClearWidgetClass) == true)
        {
            USGameResultWidget_Single* GameClearWidgetInstance =
                CreateWidget<USGameResultWidget_Single>(this, GameClearWidgetClass);
            if (IsValid(GameClearWidgetInstance) == true)
            {
                GameClearWidgetInstance->AddToViewport(5);
                GameClearWidgetInstance->SetVisibility(ESlateVisibility::Visible);


                FInputModeUIOnly Mode;
                Mode.SetWidgetToFocus(GameClearWidgetInstance->GetCachedWidget());
                SetInputMode(Mode);

                bShowMouseCursor = true;

                // 게임 일시정지
                SetPause(true);
            }
        }
    }), 2.0f, false);
}

void ASPlayerController::OnCurrentDeathCountReachedMax()
{
    // 2초 딜레이 후 게임 오버 위젯 화면에 추가
    FTimerHandle gameTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(gameTimerHandle, FTimerDelegate::CreateLambda([&]()
    {
        if (IsValid(GameOverWidgetClass) == true)
        {
            USGameResultWidget_Single* GameOverWidgetInstance =
                CreateWidget<USGameResultWidget_Single>(this, GameOverWidgetClass);
            if (IsValid(GameOverWidgetInstance) == true)
            {
                GameOverWidgetInstance->AddToViewport(5);
                GameOverWidgetInstance->SetVisibility(ESlateVisibility::Visible);


                FInputModeUIOnly Mode;
                Mode.SetWidgetToFocus(GameOverWidgetInstance->GetCachedWidget());
                SetInputMode(Mode);

                bShowMouseCursor = true;

                // 게임 일시정지
                SetPause(true);
            }
        }
    }), 2.0f, false);
}
