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
	// 'WeaponClassNumber'에 따라 위젯 설정
	void SetWeaponClassNumber(int32 InWeaponClassNumber);

protected:
	// 'OnCurrentBulletCountChangedDelegate' 델리게이트에 바인드할 함수
	UFUNCTION()
	void OnCurrentBulletCountChanged(int32 InWeaponClassNumber, int32 InCurrentBulletCount);

protected:
	// 해당 위젯의 무기 클래스 번호
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 WeaponClassNumber;

	// 무기 클래스 번호 텍스트 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> WeaponClassNumberText;

	// 현재 총알 개수 텍스트 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentBulletCountText;
	// 최대 총알 개수 텍스트 위젯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxBulletCountText;

};
