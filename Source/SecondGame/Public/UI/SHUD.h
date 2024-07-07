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
	// 'StatComponent' 속성을 바인드할 함수
	void BindStatComponent(class USStatComponent* InStatComponent);
	// 'PlayerState' 속성을 바인드할 함수
	void BindPlayerState(class ASPlayerState* InPlayerState);

	// 'WeaponSlot' 속성들을 세팅할 함수
	void SetWeaponSlot();

protected:
	// 'OnCurrentKillCountChangedDelegate' 델리게이트에 바인드할 함수
	UFUNCTION()
	void OnKillCountChanged(int32 InOldKillCount, int32 InNewKillCount);
	// 'OnCurrentDeathCountChangedDelegate' 델리게이트에 바인드할 함수
	UFUNCTION()
	void OnDeathCountChanged(int32 InOldDeathCount, int32 InNewDeathCount);
	// 'OnWeaponClassNumberChangedDelegate' 델리게이트에 바인드할 함수
	UFUNCTION()
	void OnWeaponClassNumberChanged(int32 InWeaponClassNumber);

protected:
	// StatComponent 관련 위젯 -> HPBar
	TWeakObjectPtr<USStatComponent> StatComponent;
	// PlayerState 관련 위젯 
	// -> PlayerNameText, CurrentKillCountText, MaxKillCountText, CurrentDeathCountText, MaxDeathCountText
	TWeakObjectPtr<ASPlayerState> PlayerState;

	// 현재 킬 수 텍스트 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentKillCountText;
	// 최대 킬 수 텍스트 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxKillCountText;

	// 현재 데스 수 텍스트 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentDeathCountText;
	// 최대 데스 수 텍스트 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxDeathCountText;

	// 플레이어 이름 텍스트 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;

	// HP바 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USBW_HPBar> HPBar;

	// 무기 퀵슬롯 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USWeaponSlot> PistolSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USWeaponSlot> AssaultRifleSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USWeaponSlot> SniperRifleSlot;

	// 현재 무기 퀵슬롯 번호
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 WeaponClassNumber;
};
