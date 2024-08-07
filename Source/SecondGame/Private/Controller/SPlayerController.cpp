// SPlayerController.cpp

#include "Controller/SPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/SHUD.h"
#include "Game/SPlayerState.h"
#include "Component/SStatComponent.h"
#include "Character/SCharacter.h"
#include "UI/SGameResultWidget_Single.h"

void ASPlayerController::ToggleManualWidget()
{
    checkf(IsValid(ManualWidgetInstance) == true, TEXT("Invalid MaualWidgetInstance"));

    // 조작법 위젯 켜기
    if (false == bIsManualOn)
    {
        ManualWidgetInstance->SetVisibility(ESlateVisibility::Visible);
    }
    // 조작법 위젯 끄기
    else
    {
        ManualWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
    }

    bIsManualOn = !bIsManualOn;
}

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

    // 'SPlayerState'의 델리게이트에 멤버함수 바인드
    ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
    if (IsValid(SPlayerState) == true)
    {
        SPlayerState->OnCurrentKillCountReachedMaxDelegate.AddDynamic(this, &ThisClass::OnCurrentKillCountReachedMax);
        SPlayerState->OnCurrentDeathCountReachedMaxDelegate.AddDynamic(this, &ThisClass::OnCurrentDeathCountReachedMax);
    
        SPlayerState->OnCurrentKillCountChangedDelegate.AddDynamic(this, &ThisClass::OnCurrentKillCountChanged);
        SPlayerState->OnCurrentDeathCountChangedDelegate.AddDynamic(this, &ThisClass::OnCurrentDeathCountChanged);
    }

    // 조작법 위젯 객체 생성
    if (IsValid(ManualWidgetClass) == true)
    {
        ManualWidgetInstance = CreateWidget<UUserWidget>(this, ManualWidgetClass);
        if (IsValid(ManualWidgetInstance) == true)
        {
            ManualWidgetInstance->AddToViewport(3);
            ManualWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    // 알림창 위젯 생성
    if (IsValid(NotificationWidgetClass) == true)
    {
        UUserWidget* NotificationWidgetInstance = CreateWidget<UUserWidget>(this, NotificationWidgetClass);
        if (IsValid(NotificationWidgetInstance) == true)
        {
            NotificationWidgetInstance->AddToViewport(1);
            NotificationWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void ASPlayerController::OnCurrentKillCountReachedMax()
{
	// 게임 클리어 위젯 화면에 추가
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
		}
	}
}

void ASPlayerController::OnCurrentDeathCountReachedMax()
{
	// 게임 오버 위젯 화면에 추가
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
		}
	}
}

void ASPlayerController::OnCurrentKillCountChanged(int32 InOldCurrentKillCount, int32 InNewCurrentKillCount)
{
    NotificationText = FText::FromString(TEXT("You killed an NPC"));
}

void ASPlayerController::OnCurrentDeathCountChanged(int32 InOldCurrentDeathCount, int32 InNewCurrentDeathCount)
{
    NotificationText = FText::FromString(TEXT("You were killed by an NPC"));
}
