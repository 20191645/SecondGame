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

	// �Ӽ��� �ʱ�ȭ
	Velocity = FVector::ZeroVector;
	CurrentSpeed = 0.f;
	bIsFalling = false;
	bIsCrouching = false;
	Acceleration = FVector::ZeroVector;
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// TryGetPawnOwner(): AnimInstance�� ����, �� Pawn Owner �������� �õ�
	ASCharacter* OwnerCharacter = Cast<ASCharacter>(TryGetPawnOwner());
	if (IsValid(OwnerCharacter) == true)
	{
		UCharacterMovementComponent* CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
		if (IsValid(CharacterMovementComponent) == true)
		{
			// �Ӽ��� ������Ʈ
			Velocity = CharacterMovementComponent->GetLastUpdateVelocity();
			CurrentSpeed = Velocity.Size();
			bIsFalling = CharacterMovementComponent->IsFalling();
			bIsCrouching = CharacterMovementComponent->IsCrouching();
			Acceleration = CharacterMovementComponent->GetCurrentAcceleration();

			// 'LocomotionState'�� State ����
			if (Acceleration.Length() < KINDA_SMALL_NUMBER && Velocity.Length() < KINDA_SMALL_NUMBER)
			{
				LocomotionState = ELocomotionState::Idle;
			}
			else
			{
				LocomotionState = ELocomotionState::Walk;
			}

			// 'MovementDirection'�� ���� ����
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

				// ��Ʈ�� �����̼� �� ������Ʈ
				ControlRotation.Pitch = OwnerPlayerCharacter->GetCurrentAimPitch();
				ControlRotation.Yaw = OwnerPlayerCharacter->GetCurrentAimYaw();
			}
		}
	}
}

void USAnimInstance::AnimNotify_FireEffect()
{
	// �ش� ��������Ʈ�� 1���� �Լ��� ���ε� �Ǿ� �ִٸ� true�� ��ȯ
	if (OnFireEffect.IsBound() == true)
	{
		// ��������Ʈ�� ���ε�� �Լ��鿡�� Broadcast
		OnFireEffect.Broadcast();
	}
}
