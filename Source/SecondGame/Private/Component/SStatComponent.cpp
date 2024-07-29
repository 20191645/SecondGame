// SStatComponent.cpp

#include "Component/SStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

USStatComponent::USStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	// 'PostInitializeComponents() �Լ� ȣ��X
	bWantsInitializeComponent = false;
}

void USStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 'MaxHP, CurrentHP' �ʱ�ȭ
	float InitialMaxHP = 200.f;
	SetMaxHP(InitialMaxHP);
	SetCurrentHP(InitialMaxHP);
}

void USStatComponent::SetMaxHP(float InMaxHP)
{
	if (true == OnMaxHPChangedDelegate.IsBound())
	{
		OnMaxHPChangedDelegate.Broadcast(MaxHP, InMaxHP);
	}

	MaxHP = FMath::Clamp<float>(InMaxHP, 0.f, 1000.f);
}

void USStatComponent::SetCurrentHP(float InCurrentHP)
{
	if (true == OnCurrentHPChangedDelegate.IsBound())
	{
		OnCurrentHPChangedDelegate.Broadcast(CurrentHP, InCurrentHP);
	}

	CurrentHP = FMath::Clamp<float>(InCurrentHP, 0.f, MaxHP);

	// 'CurrentHP'�� 0�� ��������� ��
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		OnOutOfCurrentHPDelegate.Broadcast();
		CurrentHP = 0.f;
	}

	// 'CurrentHP' ��ȭ ������Ʈ
	OnCurrentHPChanged_NetMulticast(CurrentHP, CurrentHP);
}

void USStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �ش� �Ӽ� �����ؼ� ����
	DOREPLIFETIME(ThisClass, MaxHP);
	DOREPLIFETIME(ThisClass, CurrentHP);
}

void USStatComponent::OnCurrentHPChanged_NetMulticast_Implementation(float InOldCurrentHP, float InNewCurrentHP)
{
	if (true == OnCurrentHPChangedDelegate.IsBound())
	{
		OnCurrentHPChangedDelegate.Broadcast(InOldCurrentHP, InNewCurrentHP);
	}

	if (InNewCurrentHP < KINDA_SMALL_NUMBER)
	{
		OnOutOfCurrentHPDelegate.Broadcast();
	}
}
