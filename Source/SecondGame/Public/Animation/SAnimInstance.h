// SAnimInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireEffect);

// 'LocomotionSM'의 State
UENUM(BlueprintType)
enum class ELocomotionState : uint8
{
	None,
	Idle,	// 멈춤
	Walk,	// 걷기
	End
};

// 캐릭터가 움직이는 방향
UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	None,
	Fwd,	// 앞
	Bwd,	// 뒤
	Left,	// 왼
	Right,	// 오
	End
};

UCLASS()
class SECONDGAME_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USAnimInstance();

	// NativeInitializeAnimation(): 초기화 함수
	virtual void NativeInitializeAnimation() override;

	// NativeUpdateAnimation(): AnimInstance의 Tick() 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 'LocomotionState, MovementDirection' Getter 함수
	ELocomotionState GetLocomotionState() const { return LocomotionState; }
	EMovementDirection GetMovementDirection() const { return MovementDirection; }

	// 'bIsDead' Getter 함수
	uint8 GetbIsDead() const { return bIsDead; }

protected:
	// Notify 'FireEffect'가 배치된 프레임에 호출할 함수
	UFUNCTION()
	void AnimNotify_FireEffect();

public:
	// 'FOnFireEffect' 델리게이트
	FOnFireEffect OnFireEffect;

protected:
	// 현재 캐릭터의 속력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	float CurrentSpeed;
	// 현재 캐릭터의 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	FVector Velocity;

	// 캐릭터가 현재 공중에 떠있는지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	uint8 bIsFalling : 1;
	// 캐릭터가 현재 쭈그려 앉아있는지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	uint8 bIsCrouching : 1;

	// 현재 캐릭터의 가속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	FVector Acceleration;

	// 'ELocomotionState' 속성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELocomotionState LocomotionState;
	// 'EMovementDirection' 속성
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EMovementDirection MovementDirection;

	// 컨트롤 로테이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	FRotator ControlRotation;

	// 캐릭터가 현재 죽었는지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SAnimInstance", meta = (AllowPrivateAccess))
	uint8 bIsDead : 1;
};
