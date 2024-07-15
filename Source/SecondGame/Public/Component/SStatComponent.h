// SStatComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfCurrentHPDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentHPChangeDelegate, float, InOldCurrentHP, float, InNewCurrentHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHPChangeDelegate, float, InOldMaxHP, float, InNewMaxHP);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SECONDGAME_API USStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USStatComponent();

	virtual void BeginPlay() override;

	// 'MaxHP' Getter, Setter �Լ�
	float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(float InMaxHP);

	// 'CurrentHP' Getter, Setter �Լ�
	float GetCurrentHP() const { return CurrentHP; }
	void SetCurrentHP(float InCurrentHP);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	// 'CurrentHP' ��ȭ ������Ʈ
	UFUNCTION(NetMulticast, Reliable)
	void OnCurrentHPChanged_NetMulticast(float InOldCurrentHP, float InNewCurrentHP);

public:
	// 'CurrentHP'�� 0�� ��������� BroadCast�ϴ� ��������Ʈ
    FOnOutOfCurrentHPDelegate OnOutOfCurrentHPDelegate;
	// 'CurrentHP'�� ��ȭ�ϸ� BroadCast�ϴ� ��������Ʈ
    FOnCurrentHPChangeDelegate OnCurrentHPChangedDelegate;
	// 'MaxHP'�� ��ȭ�ϸ� BroadCast�ϴ� ��������Ʈ
    FOnMaxHPChangeDelegate OnMaxHPChangedDelegate;

private:
	// ĳ������ �ִ� HP
    UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    float MaxHP;
	// ĳ������ ���� HP
    UPROPERTY(Replicated, Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    float CurrentHP;
};
