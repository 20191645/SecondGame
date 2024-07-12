// SNonPlayerCharacter.cpp

#include "Character/SNonPlayerCharacter.h"
#include "Controller/SAIController.h"
#include "Item/SWeaponActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/SAnimInstance.h"
#include "Engine/DamageEvents.h"
#include "Character/SPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Component/SStatComponent.h"
#include "Game/SPlayerState.h"

ASNonPlayerCharacter::ASNonPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// NPC�� ������ ������ SAIController ����
	AIControllerClass = ASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// ���� ��� �ð� ���� �ʱ�ȭ
	TimeBetweenFire = 60.f / FirePerMinute;
}

void ASNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� 'WeaponSocket'�� ���� ���� ����
	FName WeaponSocket(TEXT("WeaponSocket01"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
	{
		// ���� ���� ����
		WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (IsValid(WeaponInstance) == true)
		{
			WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		}
	}

	// IsPlayerControllered(): �÷��̾ �����ϰ� �ִ� ������ Ȯ��
	if (false == IsPlayerControlled())
	{
		// bUseControllerRotation***: ControlRotation�� ���� ĳ���Ͱ� ȸ������ �ʴ´� [false]
		bUseControllerRotationYaw = false;

		// bOrientRotationToMovement: ĳ���� �̵� ���⿡ ȸ���� ��ġ��Ű�� �ʴ´� [false]
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// bUseControllerDesiredRotation: ControlRotation�� ��ǥ ȸ�������� ��Ƽ� ȸ���Ѵ� [true]
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// RotationRate: ȸ�� �ӵ�
		GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

		// NPC �̵��ӵ� ����
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}

	// 'OnFireEffect' ��������Ʈ�� OnFireEffect() �Լ� ���ε�
	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance) == true)
	{
		AnimInstance->OnFireEffect.AddDynamic(this, &ThisClass::OnFireEffect);
	}

	// 'BulletCount' Key �� ��������
	ASAIController* AIController = GetController<ASAIController>();
	if (IsValid(AIController) == true) {
		MaxBulletCount=
			AIController->GetBlackboardComponent()->GetValueAsInt(AIController->BulletCountKey);
	}
}

void ASNonPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ASAIController* AIController = GetController<ASAIController>();
	if (IsValid(AIController) == true) {
		if (ASPlayerCharacter* TargetPC =
			Cast<ASPlayerCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(AIController->TargetActorKey)))
		{
			// WeaponMuzzleLocation: ���� ������ 'MuzzleFlash' ���� ��ġ
			FVector WeaponMuzzleLocation = WeaponInstance->GetMesh()->GetSocketLocation(TEXT("MuzzleFlash"));
			// �÷��̾� ĳ������ �Ӹ� bone ��ġ
			FVector PlayerLocation = TargetPC->GetMesh()->GetSocketLocation(TEXT("head"));
			
			// TargetRotation: NPC ȸ�� ��ǥġ ����
			FVector LookVector = PlayerLocation - WeaponMuzzleLocation;
			LookVector.Z = 0.f;
			FRotator TargetRotation = FRotationMatrix::MakeFromX(LookVector).Rotator();

			// NPC �ε巴�� ȸ��
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.f));
		
			// 'CurrentAimPitch, CurrentAimYaw' �Ӽ��� ������Ʈ
			TargetRotation = UKismetMathLibrary::FindLookAtRotation(WeaponMuzzleLocation, PlayerLocation);
			CurrentAimPitch = TargetRotation.Pitch;
			CurrentAimYaw = GetActorRotation().Yaw;

			AIController->SetControlRotation(FRotator(CurrentAimPitch, CurrentAimYaw, AIController->GetControlRotation().Roll));
		}
	}
}

float ASNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinialDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// NPC ����
	if (GetStatComponent()->GetCurrentHP() < KINDA_SMALL_NUMBER)
	{
		// NPC�� ���� �÷��̾��� 'CurrentKillCount' �� ����
		ASPlayerCharacter* DamageCauserCharacter = Cast<ASPlayerCharacter>(DamageCauser);
		if (IsValid(DamageCauserCharacter) == true)
		{
			ASPlayerState* DCCPlayerState = Cast<ASPlayerState>(DamageCauserCharacter->GetPlayerState());
			if (IsValid(DCCPlayerState) == true)
			{
				DCCPlayerState->AddCurrentKillCount(1);
			}
		}
		
		// ���� ��� Ÿ�̸� Ŭ����
		GetWorldTimerManager().ClearTimer(BetweenShotsTimer);

		// Behavior Tree ����
		ASAIController* AIController = Cast<ASAIController>(GetController());
		if (true == ::IsValid(AIController))
		{
			AIController->EndAI();
		}

		// �״� �ִϸ��̼� ��� ������ �� �ı�
		FTimerHandle deathTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
			WeaponInstance->Destroy();
			WeaponInstance = nullptr;
		}), 1.2f, false);
	}
	// �ǰ� ����
	else {
		// �ǰ� �ִϸ��̼� ���
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance) && IsValid(HitReactAnimMontage))
		{
			AnimInstance->Montage_Play(HitReactAnimMontage);
		}
	}

	return FinialDamageAmount;
}

void ASNonPlayerCharacter::TryFire()
{
	// ĳ���Ͱ� ���� ���̰ų� �ǰ� ���̸� ���� �Ұ�
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (GetCharacterMovement()->IsFalling() == true ||
		AnimInstance->Montage_IsPlaying(HitReactAnimMontage) == true)
	{
		return;
	}

	ASAIController* AIController = GetController<ASAIController>();
	if (IsValid(AIController) == true && IsValid(WeaponInstance) == true) {

		if (ASPlayerCharacter* TargetPC = 
			Cast<ASPlayerCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject(AIController->TargetActorKey)))
		{

#pragma region BulletCount
			// �Ѿ� ���� �ִϸ��̼� ��� ���̸� return
			if (IsValid(AnimInstance) == false ||
				AnimInstance->Montage_IsPlaying(WeaponInstance->GetReloadAnimMontage()) == true) {
				return;
			}

			// �Ѿ� ���� Ȯ��
			int32 BulletCount = AIController->GetBlackboardComponent()->GetValueAsInt(AIController->BulletCountKey);
			if (BulletCount == 0) {
				return;
			}
			// �Ѿ� ���
			AIController->GetBlackboardComponent()->SetValueAsInt(AIController->BulletCountKey, BulletCount - 1);
#pragma endregion

#pragma region CaculateTargetTransform
			// WeaponMuzzleLocation: ���� ������ 'MuzzleFlash' ���� ��ġ
			FVector WeaponMuzzleLocation = WeaponInstance->GetMesh()->GetSocketLocation(TEXT("MuzzleFlash"));
			// Tick() �Լ����� ������Ʈ�ϴ� 'ControlRotation' ��
			FRotator ControlRotation = AIController->GetControlRotation();

			// TargetTransform: ��� ���� ����
			FTransform TargetTransform = FTransform(ControlRotation, WeaponMuzzleLocation);
#pragma endregion

#pragma region LineTracing
		// BulletDirection: ��� ���� -- TargetTransform�� Forward ����
		FVector BulletDirection = TargetTransform.GetUnitAxis(EAxis::X);
		// StartLocation: ��� ������ ��ġ (=FinalFocalLocation)
		FVector StartLocation = TargetTransform.GetLocation();
		// EndLocation: (��� ������ + ��Ÿ�) ��ġ
		FVector EndLocation = StartLocation + BulletDirection * WeaponInstance->GetMaxRange();

		// HitResult: �浹 Ž�� ���
		FHitResult HitResult;
		// TraceParams: �浹 Ž�� ���
		FCollisionQueryParams TraceParams(NAME_None, false, this);
		// �浹 Ž�� ��󿡼� ���� ���ʹ� ����
		TraceParams.AddIgnoredActor(WeaponInstance);

		// LineTraceSingleByChannel: StartLocation���� EndLocation ������ ��ġ���� TraceParams �� 'Attack' ä���� ����Ǵ� �浹 Ž�� ����� HitResult�� ����
		bool IsCollided = 
			GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel2, TraceParams);
		// �浹 ���� ���� �� TraceStart, TraceEnd �� ���� ����
		if (IsCollided == false)
		{
			HitResult.TraceStart = StartLocation;
			HitResult.TraceEnd = EndLocation;
		}

		// ��� �Ÿ� ����� �����
		DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Red, false, 0.5f, 0, 0.5f);
#pragma endregion

#pragma region DamageEvent
		if (IsCollided == true)
		{
			// ��� ������ �÷��̾� ĳ���Ϳ��� ������ ������
			ASPlayerCharacter* HittedCharacter = Cast<ASPlayerCharacter>(HitResult.GetActor());
			if (IsValid(HittedCharacter) == true)
			{
				FDamageEvent DamageEvent;

				// ��� ������ ĳ���Ϳ��� ���������� ����� ������
				float HitDamage = 10.f;

				// ��弦 ���� �� ������ 2�� ����
				FString BoneNameString = HitResult.BoneName.ToString();
				if (true == BoneNameString.Equals(FString(TEXT("HEAD")), ESearchCase::IgnoreCase))
				{
					HitDamage *= 2;
				}

				HittedCharacter->TakeDamage(HitDamage, DamageEvent, GetController(), this);
			}
		}

		// ��� �ִϸ��̼� ���
		if (IsValid(WeaponInstance->GetFireAnimMontage()))
		{
			AnimInstance->Montage_Play(WeaponInstance->GetFireAnimMontage());
		}
#pragma endregion
		}

	}
}

void ASNonPlayerCharacter::OnFireEffect()
{
	FName WeaponSocket(TEXT("WeaponSocket01"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == true)
	{
		FRotator ParticleRotation(75.f, 0.f, 0.f);
		// WeaponSocket���� �ѱ������� �Ÿ� ����
		FVector ParticleLocation = FVector(-15.f, 65.f, 0.f);

		// 'FireParticleSystem' ȿ�� ���
		if (IsValid(WeaponInstance->GetFireParticleSystem())) {
			UGameplayStatics::SpawnEmitterAttached(
				WeaponInstance->GetFireParticleSystem(),
				GetMesh(),
				WeaponSocket,
				ParticleLocation,
				ParticleRotation,
				(FVector)(0.3f)
			);
		}
	}
}

void ASNonPlayerCharacter::Reload()
{
	// ���� �ִϸ��̼� ��� ���̸� return
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false ||
		AnimInstance->Montage_IsPlaying(WeaponInstance->GetFireAnimMontage()) == true) {
		return;
	}

	// �Ѿ� ���� �ִϸ��̼� ���
	if (IsValid(WeaponInstance->GetReloadAnimMontage()) == true)
	{
		AnimInstance->Montage_Play(WeaponInstance->GetReloadAnimMontage());
	}

	// �Ѿ� ���� ����
	ASAIController* AIController = GetController<ASAIController>();
	if (IsValid(AIController) == true) {
		AIController->GetBlackboardComponent()->SetValueAsInt(AIController->BulletCountKey, MaxBulletCount);
	}
}

void ASNonPlayerCharacter::StartFire()
{
	// ���� ��� Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(BetweenShotsTimer, this, &ThisClass::TryFire, TimeBetweenFire, true);
}
