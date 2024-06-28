// SWeaponActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeaponActor.generated.h"

class UAnimInstance;
class UAnimMontage;

UCLASS()
class SECONDGAME_API ASWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASWeaponActor();

	// 'Mesh' Getter 함수
	USkeletalMeshComponent* GetMesh() const { return Mesh; }

	// 'UnarmedCharacterAnimLayer, PistolCharacterAnimLayer, RifleCharacterAnimLayer' Getter 함수
	TSubclassOf<UAnimInstance> GetUnarmedCharacterAnimLayer() const { return UnarmedCharacterAnimLayer; }
	TSubclassOf<UAnimInstance> GetPistolCharacterAnimLayer() const { return PistolCharacterAnimLayer; }
	TSubclassOf<UAnimInstance> GetRifleCharacterAnimLayer() const { return RifleCharacterAnimLayer; }

	// 'EquipAnimMontage, UnequipAnimMontage' Getter 함수
	UAnimMontage* GetEquipAnimMontage() const { return EquipAnimMontage; }
	UAnimMontage* GetUnequipAnimMontage() const { return UnequipAnimMontage; }

protected:
	// 무기 아이템이 사용할 Mesh 정보
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	// 무기 해제 시 사용할 Animation Layer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASWeaponActor|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> UnarmedCharacterAnimLayer;
	// 피스톨 무기 장착 시 사용할 Animation Layer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASWeaponActor|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> PistolCharacterAnimLayer;
	// 라이플 무기 장착 시 사용할 Animation Layer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASWeaponActor|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> RifleCharacterAnimLayer;

	// 무기 장착 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> EquipAnimMontage;
	// 무기 탈착 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> UnequipAnimMontage;
};
