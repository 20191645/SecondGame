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

	USkeletalMeshComponent* GetMesh() const { return Mesh; }

	TSubclassOf<UAnimInstance> GetUnarmedCharacterAnimLayer() const { return UnarmedCharacterAnimLayer; }
	TSubclassOf<UAnimInstance> GetPistolCharacterAnimLayer() const { return PistolCharacterAnimLayer; }
	TSubclassOf<UAnimInstance> GetRifleCharacterAnimLayer() const { return RifleCharacterAnimLayer; }

	UAnimMontage* GetEquipAnimMontage() const { return EquipAnimMontage; }
	UAnimMontage* GetUnequipAnimMontage() const { return UnequipAnimMontage; }

protected:
	// ���� �������� ����� Mesh ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> Mesh;

	// ���� ���� �� ����� Animation Layer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASWeaponActor|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> UnarmedCharacterAnimLayer;
	// �ǽ��� ���� ���� �� ����� Animation Layer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASWeaponActor|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> PistolCharacterAnimLayer;
	// ������ ���� ���� �� ����� Animation Layer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ASWeaponActor|AnimLayer", meta = (AllowPrivateAccess))
	TSubclassOf<UAnimInstance> RifleCharacterAnimLayer;

	// ���� ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> EquipAnimMontage;
	// ���� Ż�� �ִϸ��̼�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> UnequipAnimMontage;
};
