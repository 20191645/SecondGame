// SWeaponSlot.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWeaponSlot.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class SECONDGAME_API USWeaponSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 'WeaponClassNumber'�� ���� ���� ����
	void SetWeaponClassNumber(int32 InWeaponClassNumber);

protected:
	// 'OnCurrentBulletCountChangedDelegate' ��������Ʈ�� ���ε��� �Լ�
	UFUNCTION()
	void OnCurrentBulletCountChanged(int32 InWeaponClassNumber, int32 InCurrentBulletCount);

protected:
	// �ش� ������ ���� Ŭ���� ��ȣ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 WeaponClassNumber;

	// ���� Ŭ���� ��ȣ �ؽ�Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> WeaponClassNumberText;

	// ���� �Ѿ� ���� �ؽ�Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentBulletCountText;
	// �ִ� �Ѿ� ���� �ؽ�Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxBulletCountText;

};
