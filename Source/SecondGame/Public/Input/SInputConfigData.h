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
};
