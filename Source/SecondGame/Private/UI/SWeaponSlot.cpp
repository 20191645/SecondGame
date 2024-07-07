// SWeaponSlot.cpp

#include "UI/SWeaponSlot.h"
#include "Components/TextBlock.h"
#include "Character/SPlayerCharacter.h"

void USWeaponSlot::SetWeaponClassNumber(int32 InWeaponClassNumber)
{
	ASPlayerCharacter* OwningCharacter = Cast<ASPlayerCharacter>(GetOwningPlayerPawn());
	if (IsValid(OwningCharacter) == true) {
		// 'OnCurrentBulletCountChangedDelegate'�� ����Լ� ���ε�
		OwningCharacter->OnCurrentBulletCountChangedDelegate.AddDynamic(this, &ThisClass::OnCurrentBulletCountChanged);

		WeaponClassNumber = FMath::Clamp<int32>(InWeaponClassNumber, 1, 3);

		// 'WeaponClassNumberText' �ʱ�ȭ
		FString WeaponClassNumberString = FString::Printf(TEXT("%d"), WeaponClassNumber);
		WeaponClassNumberText->SetText(FText::FromString(WeaponClassNumberString));

		// 'CurrentBulletCountText, MaxBulletCountText' �ʱ�ȭ
		FString CurrentBulletCountString = 
			FString::Printf(TEXT("%d"), OwningCharacter->GetCurrentBulletCount(WeaponClassNumber));
		CurrentBulletCountText->SetText(FText::FromString(CurrentBulletCountString));

		FString MaxBulletCountString = 
			FString::Printf(TEXT("%d"), OwningCharacter->GetMaxBulletCount(WeaponClassNumber));
		MaxBulletCountText->SetText(FText::FromString(MaxBulletCountString));
	}
}

void USWeaponSlot::OnCurrentBulletCountChanged(int32 InWeaponClassNumber, int32 InCurrentBulletCount)
{
	if (WeaponClassNumber == InWeaponClassNumber) {
		// 'CurrentBulletCountText' ������Ʈ
		FString CurrentBulletCountString = FString::Printf(TEXT("%d"), InCurrentBulletCount);
		CurrentBulletCountText->SetText(FText::FromString(CurrentBulletCountString));
	}
}