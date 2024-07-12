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

    // 'OnCurrentKillCountReachedMaxDelegate, OnCurrentDeathCountReachedMaxDelegate' ��������Ʈ�� ����Լ� ���ε�
    ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
    if (IsValid(SPlayerState) == true)
    {
        SPlayerState->OnCurrentKillCountReachedMaxDelegate.AddDynamic(this, &ThisClass::OnCurrentKillCountReachedMax);
        SPlayerState->OnCurrentDeathCountReachedMaxDelegate.AddDynamic(this, &ThisClass::OnCurrentDeathCountReachedMax);
    }
}

void ASPlayerController::OnCurrentKillCountReachedMax()
{
    // 2�� ������ �� ���� Ŭ���� ���� ȭ�鿡 �߰�
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

                // ���� �Ͻ�����
                SetPause(true);
            }
        }
    }), 2.0f, false);
}

void ASPlayerController::OnCurrentDeathCountReachedMax()
{
    // 2�� ������ �� ���� ���� ���� ȭ�鿡 �߰�
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

                // ���� �Ͻ�����
                SetPause(true);
            }
        }
    }), 2.0f, false);
}
