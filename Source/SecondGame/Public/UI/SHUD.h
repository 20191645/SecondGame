// SHUD.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHUD.generated.h"

class USStatComponent;
class ASPlayerState;
class UTextBlock;
class USBW_HPBar;
class USWeaponSlot;

UCLASS()
class SECONDGAME_API USHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// 'StatComponent' �Ӽ��� ���ε��� �Լ�
	void BindStatComponent(class USStatComponent* InStatComponent);
	// 'PlayerState' �Ӽ��� ���ε��� �Լ�
	void BindPlayerState(class ASPlayerState* InPlayerState);

	// 'WeaponSlot' �Ӽ����� ������ �Լ�
	void SetWeaponSlot();

protected:
	// 'OnCurrentKillCountChangedDelegate' ��������Ʈ�� ���ε��� �Լ�
	UFUNCTION()
	void OnKillCountChanged(int32 InOldKillCount, int32 InNewKillCount);
	// 'OnCurrentDeathCountChangedDelegate' ��������Ʈ�� ���ε��� �Լ�
	UFUNCTION()
	void OnDeathCountChanged(int32 InOldDeathCount, int32 InNewDeathCount);
	// 'OnWeaponClassNumberChangedDelegate' ��������Ʈ�� ���ε��� �Լ�
	UFUNCTION()
	void OnWeaponClassNumberChanged(int32 InWeaponClassNumber);

protected:
	// StatComponent ���� ���� -> HPBar
	TWeakObjectPtr<USStatComponent> StatComponent;
	// PlayerState ���� ���� 
	// -> PlayerNameText, CurrentKillCountText, MaxKillCountText, CurrentDeathCountText, MaxDeathCountText
	TWeakObjectPtr<ASPlayerState> PlayerState;

	// ���� ų �� �ؽ�Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentKillCountText;
	// �ִ� ų �� �ؽ�Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxKillCountText;

	// ���� ���� �� �ؽ�Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentDeathCountText;
	// �ִ� ���� �� �ؽ�Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxDeathCountText;

	// �÷��̾� �̸� �ؽ�Ʈ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;

	// HP�� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USBW_HPBar> HPBar;

	// ���� ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USWeaponSlot> PistolSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USWeaponSlot> AssaultRifleSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USWeaponSlot> SniperRifleSlot;

	// ���� ���� ������ ��ȣ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 WeaponClassNumber;
};
