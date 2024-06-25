// SWeaponActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeaponActor.generated.h"

UCLASS()
class SECONDGAME_API ASWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASWeaponActor();

	USkeletalMeshComponent* GetMesh() const { return Mesh; }

protected:
	// ���� �������� ����� Mesh ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> Mesh;
};
