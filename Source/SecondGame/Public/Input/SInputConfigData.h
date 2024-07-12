// SInputConfigData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SInputConfigData.generated.h"

class UInputAction;

UCLASS()
class SECONDGAME_API USInputConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 'IA_Move'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Move;

	// 'IA_Look'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Look;

	// 'IA_Jump'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Jump;

	// 'IA_QuickSlot01'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> QuickSlot01;

	// 'IA_QuickSlot02'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> QuickSlot02;

	// 'IA_QuickSlot03'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> QuickSlot03;

	// 'IA_Crouch'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Crouch;

	// 'IA_Attack'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Attack;

	// 'IA_Zoom'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Zoom;

	// 'IA_Trigger'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Trigger;

	// 'IA_Reload'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Reload;

	// 'IA_Manual'에 대응하는 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Manual;
};
