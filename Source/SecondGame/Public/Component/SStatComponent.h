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

	// 'MaxHP' Getter, Setter 함수
	float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(float InMaxHP);

	// 'CurrentHP' Getter, Setter 함수
	float GetCurrentHP() const { return CurrentHP; }
	void SetCurrentHP(float InCurrentHP);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

private:
	// 'CurrentHP' 변화 업데이트
	UFUNCTION(NetMulticast, Reliable)
	void OnCurrentHPChanged_NetMulticast(float InOldCurrentHP, float InNewCurrentHP);

public:
	// 'CurrentHP'가 0에 가까워지면 BroadCast하는 델리게이트
    FOnOutOfCurrentHPDelegate OnOutOfCurrentHPDelegate;
	// 'CurrentHP'가 변화하면 BroadCast하는 델리게이트
    FOnCurrentHPChangeDelegate OnCurrentHPChangedDelegate;
	// 'MaxHP'가 변화하면 BroadCast하는 델리게이트
    FOnMaxHPChangeDelegate OnMaxHPChangedDelegate;

private:
	// 캐릭터의 최대 HP
    UPROPERTY(Replicated, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    float MaxHP;
	// 캐릭터의 현재 HP
    UPROPERTY(Replicated, Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "USStatComponent", Meta = (AllowPrivateAccess))
    float CurrentHP;
};
