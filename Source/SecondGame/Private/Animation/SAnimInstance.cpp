// SAnimInstance.cpp

#include "Animation/SAnimInstance.h"
#include "Character/SPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

USAnimInstance::USAnimInstance()
{
}

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 속성값 초기화
	Velocity = FVector::ZeroVector;
	CurrentSpeed = 0.f;
	bIsFalling = false;
	bIsCrouching = false;
	Acceleration = FVector::ZeroVector;
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// TryGetPawnOwner(): AnimInstance의 주인, 즉 Pawn Owner 가져오기 시도
	ASCharacter* OwnerCharacter = Cast<ASCharacter>(TryGetPawnOwner());
	if (IsValid(OwnerCharacter) == true)
	{
		UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
		if (IsValid(CharacterMovementComponent) == true)
		{
			// 속성값 업데이트
			Velocity = CharacterMovementComponent->GetLastUpdateVelocity();
			CurrentSpeed = Velocity.Size();
			bIsFalling = CharacterMovementComponent->IsFalling();
			bIsCrouching = CharacterMovementComponent->IsCrouching();
			Acceleration = CharacterMovementComponent->GetCurrentAcceleration();

			// 'LocomotionState'의 State 설정
			if (Acceleration.Length() < KINDA_SMALL_NUMBER && Velocity.Length() < KINDA_SMALL_NUMBER)
			{
				LocomotionState = ELocomotionState::Idle;
			}
			else
			{
				LocomotionState = ELocomotionState::Walk;
			}

			// 'MovementDirection'의 방향 설정
			ASPlayerCharacter* OwnerPlayerCharacter = Cast<ASPlayerCharacter>(OwnerCharacter);
			if (IsValid(OwnerPlayerCharacter) == true)
			{
				if (KINDA_SMALL_NUMBER < OwnerPlayerCharacter->GetForwardInputValue())
				{
					MovementDirection = EMovementDirection::Fwd;
				}

				if (OwnerPlayerCharacter->GetForwardInputValue() < -KINDA_SMALL_NUMBER)
				{
					MovementDirection = EMovementDirection::Bwd;
				}

				if (KINDA_SMALL_NUMBER < OwnerPlayerCharacter->GetRightInputValue())
				{
					MovementDirection = EMovementDirection::Right;
				}

				if (OwnerPlayerCharacter->GetRightInputValue() < -KINDA_SMALL_NUMBER)
				{
					MovementDirection = EMovementDirection::Left;
				}

				// 컨트롤 로테이션 값 업데이트
				ControlRotation.Pitch = OwnerPlayerCharacter->GetCurrentAimPitch();
				ControlRotation.Yaw = OwnerPlayerCharacter->GetCurrentAimYaw();
			}
		}
	}
}
