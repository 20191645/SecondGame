// SNonPlayerCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "Character/SCharacter.h"
#include "SNonPlayerCharacter.generated.h"

class ASWeaponActor;

UCLASS()
class SECONDGAME_API ASNonPlayerCharacter : public ASCharacter
{
	GENERATED_BODY()
	
public:
	ASNonPlayerCharacter();

	virtual void BeginPlay() override;

protected:
	// 무기 클래스 정보 -- 'Assault_Rifle_A'
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSubclassOf<ASWeaponActor> WeaponClass;
	// 무기 액터 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<ASWeaponActor> WeaponInstance;
};
