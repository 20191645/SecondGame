// SWeaponActor.cpp

#include "Item/SWeaponActor.h"

ASWeaponActor::ASWeaponActor()
{
 	PrimaryActorTick.bCanEverTick = false;

	// 'Mesh' ������Ʈ ����
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Ŭ���̾�Ʈ ���� ���
	bReplicates = true;
}