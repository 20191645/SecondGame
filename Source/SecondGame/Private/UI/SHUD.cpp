// SHUD.cpp

#include "UI/SHUD.h"
#include "Component/SStatComponent.h"
#include "Game/SPlayerState.h"
#include "UI/SBW_HPBar.h"
#include "Components/TextBlock.h"
#include "UI/SWeaponSlot.h"
#include "Character/SPlayerCharacter.h"

void USHUD::BindStatComponent(USStatComponent* InStatComponent)
{
	if (IsValid(InStatComponent) == true)
	{
		// 'StatComponent' �ʱ�ȭ �� ��������Ʈ�� 'HPBar' ���ε�
		StatComponent = InStatComponent;
		StatComponent->OnCurrentHPChangedDelegate.AddDynamic(HPBar, &USBW_HPBar::OnCurrentHPChange);
		StatComponent->OnMaxHPChangedDelegate.AddDynamic(HPBar, &USBW_HPBar::OnMaxHPChange);

		// PlayerController::BeginPlay()�� SStatComponent::BeginPlay()���� ���� ȣ��Ǳ⿡ ���� �ʱ�ȭ
		float InitialMaxHP = 200.f;
		HPBar->SetMaxHP(InitialMaxHP);
		HPBar->InitializeHPBarWidget(StatComponent.Get());
	}
}

void USHUD::BindPlayerState(ASPlayerState* InPlayerState)
{
	if (IsValid(InPlayerState) == true)
	{
		// 'PlayerState' �ʱ�ȭ �� ��������Ʈ�� 'OnKillCountChanged, OnDeathCountChanged' ���ε�
		PlayerState = InPlayerState;
		PlayerState->OnCurrentKillCountChangedDelegate.AddDynamic(this, &ThisClass::OnKillCountChanged);
		PlayerState->OnCurrentDeathCountChangedDelegate.AddDynamic(this, &ThisClass::OnDeathCountChanged);

		// 'PlayerNameText' �ʱ�ȭ
		PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));

		// 'CurrentKillCount, CurrentDeathCount' �ʱ�ȭ
		OnKillCountChanged(0, PlayerState->GetCurrentKillCount());
		OnDeathCountChanged(0, PlayerState->GetCurrentDeathCount());

		// 'MaxKillCountText' �ʱ�ȭ
		FString MaxKillCountString = FString::Printf(TEXT("%d"), PlayerState->GetMaxKillCount());
		MaxKillCountText->SetText(FText::FromString(MaxKillCountString));
		// 'MaxDeathCountText' �ʱ�ȭ
		FString MaxDeathCountString = FString::Printf(TEXT("%d"), PlayerState->GetMaxDeathCount());
		MaxDeathCountText->SetText(FText::FromString(MaxDeathCountString));
	}
}

void USHUD::SetWeaponSlot()
{
	// 'OnWeaponClassNumberChangedDelegate'�� ����Լ� ���ε�
	ASPlayerCharacter* OwningCharacter = Cast<ASPlayerCharacter>(GetOwningPlayerPawn());
	if (IsValid(OwningCharacter) == true) {
		OwningCharacter->OnWeaponClassNumberChangedDelegate.AddDynamic(this, &ThisClass::OnWeaponClassNumberChanged);
	}

	// 'WeaponSlot' �ʱ�ȭ
	PistolSlot->SetWeaponClassNumber(1);
	AssaultRifleSlot->SetWeaponClassNumber(2);
	SniperRifleSlot->SetWeaponClassNumber(3);

	// 'WeaponClassNumber' �ʱ�ȭ
	WeaponClassNumber = 1;
}

void USHUD::OnKillCountChanged(int32 InOldKillCount, int32 InNewKillCount)
{
	// 'CurrentKillCountText' ������Ʈ
	FString CurrentKillCountString = FString::Printf(TEXT("%d"), InNewKillCount);
	CurrentKillCountText->SetText(FText::FromString(CurrentKillCountString));
}

void USHUD::OnDeathCountChanged(int32 InOldDeathCount, int32 InNewDeathCount)
{
	// 'CurrentDeathCountText' ������Ʈ
	FString CurrentDeathCountString = FString::Printf(TEXT("%d"), InNewDeathCount);
	CurrentDeathCountText->SetText(FText::FromString(CurrentDeathCountString));
}

void USHUD::OnWeaponClassNumberChanged(int32 InWeaponClassNumber)
{
	WeaponClassNumber = InWeaponClassNumber;
}
