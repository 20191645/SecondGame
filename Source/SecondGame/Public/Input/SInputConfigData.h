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
	// 'IA_Move'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Move;

	// 'IA_Look'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Look;

	// 'IA_Jump'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Jump;

	// 'IA_QuickSlot01'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> QuickSlot01;

	// 'IA_QuickSlot02'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> QuickSlot02;

	// 'IA_QuickSlot03'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> QuickSlot03;

	// 'IA_Crouch'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Crouch;

	// 'IA_Attack'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Attack;

	// 'IA_Zoom'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Zoom;

	// 'IA_Trigger'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Trigger;

	// 'IA_Reload'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Reload;

	// 'IA_Manual'�� �����ϴ� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Manual;
};
