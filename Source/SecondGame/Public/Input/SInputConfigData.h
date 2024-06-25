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
};
