// SWeaponActor.cpp

#include "Item/SWeaponActor.h"

ASWeaponActor::ASWeaponActor()
{
 	PrimaryActorTick.bCanEverTick = false;

	// 'Mesh' 오브젝트 생성
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 클라이언트 복제 허용
	bReplicates = true;
}