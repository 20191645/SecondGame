// SCharacter.cpp

#include "Character/SCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    float CharacterHalfHeight = 95.f;
    float CharacterRadius = 20.f;

    GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);
    // Collision Preset ����
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("SCharacter"));

    // PivotPosition, PivotRotation: ĸ�� ������Ʈ�� ������ ��ġ, ������ ������
    FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
    FRotator PivotRotation(0.f, -90.f, 0.f);
    GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);

    // MaxWalkSpeed: ĳ���Ͱ� Walk mode�� �� �ִ� �ӷ�
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
    // MinAnalogWalkSpeed: �����а� �Ƴ��α�(Ű���� - ���̽�ƽ) �Է� ������ ������ ���ϰ�
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    // JumpZVelocity: ĳ���Ͱ� ������ �� �ӵ�
    GetCharacterMovement()->JumpZVelocity = 700.f;
    // AirControl: ���߿� ������ �� ������ �� �ִ� ����
    GetCharacterMovement()->AirControl = 0.35f;
    // BrakingDecelerationWalking: �������� ������ �� ���ߴ� �ӵ�
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}
