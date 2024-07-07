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
		// 'StatComponent' 초기화 및 델리게이트에 'HPBar' 바인드
		StatComponent = InStatComponent;
		StatComponent->OnCurrentHPChangedDelegate.AddDynamic(HPBar, &USBW_HPBar::OnCurrentHPChange);
		StatComponent->OnMaxHPChangedDelegate.AddDynamic(HPBar, &USBW_HPBar::OnMaxHPChange);

		// PlayerController::BeginPlay()가 SStatComponent::BeginPlay()보다 먼저 호출되기에 따로 초기화
		float InitialMaxHP = 200.f;
		HPBar->SetMaxHP(InitialMaxHP);
		HPBar->InitializeHPBarWidget(StatComponent.Get());
	}
}

void USHUD::BindPlayerState(ASPlayerState* InPlayerState)
{
	if (IsValid(InPlayerState) == true)
	{
		// 'PlayerState' 초기화 및 델리게이트에 'OnKillCountChanged, OnDeathCountChanged' 바인드
		PlayerState = InPlayerState;
		PlayerState->OnCurrentKillCountChangedDelegate.AddDynamic(this, &ThisClass::OnKillCountChanged);
		PlayerState->OnCurrentDeathCountChangedDelegate.AddDynamic(this, &ThisClass::OnDeathCountChanged);

		// 'PlayerNameText' 초기화
		PlayerNameText->SetText(FText::FromString(PlayerState->GetPlayerName()));

		// 'CurrentKillCount, CurrentDeathCount' 초기화
		OnKillCountChanged(0, PlayerState->GetCurrentKillCount());
		OnDeathCountChanged(0, PlayerState->GetCurrentDeathCount());

		// 'MaxKillCountText' 초기화
		FString MaxKillCountString = FString::Printf(TEXT("%d"), PlayerState->GetMaxKillCount());
		MaxKillCountText->SetText(FText::FromString(MaxKillCountString));
		// 'MaxDeathCountText' 초기화
		FString MaxDeathCountString = FString::Printf(TEXT("%d"), PlayerState->GetMaxDeathCount());
		MaxDeathCountText->SetText(FText::FromString(MaxDeathCountString));
	}
}

void USHUD::SetWeaponSlot()
{
	// 'OnWeaponClassNumberChangedDelegate'에 멤버함수 바인드
	ASPlayerCharacter* OwningCharacter = Cast<ASPlayerCharacter>(GetOwningPlayerPawn());
	if (IsValid(OwningCharacter) == true) {
		OwningCharacter->OnWeaponClassNumberChangedDelegate.AddDynamic(this, &ThisClass::OnWeaponClassNumberChanged);
	}

	// 'WeaponSlot' 초기화
	PistolSlot->SetWeaponClassNumber(1);
	AssaultRifleSlot->SetWeaponClassNumber(2);
	SniperRifleSlot->SetWeaponClassNumber(3);

	// 'WeaponClassNumber' 초기화
	WeaponClassNumber = 1;
}

void USHUD::OnKillCountChanged(int32 InOldKillCount, int32 InNewKillCount)
{
	// 'CurrentKillCountText' 업데이트
	FString CurrentKillCountString = FString::Printf(TEXT("%d"), InNewKillCount);
	CurrentKillCountText->SetText(FText::FromString(CurrentKillCountString));
}

void USHUD::OnDeathCountChanged(int32 InOldDeathCount, int32 InNewDeathCount)
{
	// 'CurrentDeathCountText' 업데이트
	FString CurrentDeathCountString = FString::Printf(TEXT("%d"), InNewDeathCount);
	CurrentDeathCountText->SetText(FText::FromString(CurrentDeathCountString));
}

void USHUD::OnWeaponClassNumberChanged(int32 InWeaponClassNumber)
{
	WeaponClassNumber = InWeaponClassNumber;
}
