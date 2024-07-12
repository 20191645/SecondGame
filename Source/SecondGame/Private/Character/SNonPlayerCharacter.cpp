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

	// NPC가 생성될 때마다 SAIController 빙의
	AIControllerClass = ASAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 연발 사격 시간 간격 초기화
	TimeBetweenFire = 60.f / FirePerMinute;
}

void ASNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 'WeaponSocket'에 무기 액터 장착
	FName WeaponSocket(TEXT("WeaponSocket01"));
	if (GetMesh()->DoesSocketExist(WeaponSocket) == true && IsValid(WeaponInstance) == false)
	{
		// 무기 액터 스폰
		WeaponInstance = GetWorld()->SpawnActor<ASWeaponActor>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (IsValid(WeaponInstance) == true)
		{
			WeaponInstance->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		}
	}

	// IsPlayerControllered(): 플레이어가 조종하고 있는 폰인지 확인
	if (false == IsPlayerControlled())
	{
		// bUseControllerRotation***: ControlRotation에 따라 캐릭터가 회전하지 않는다 [false]
		bUseControllerRotationYaw = false;

		// bOrientRotationToMovement: 캐릭터 이동 방향에 회전을 일치시키지 않는다 [false]
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// bUseControllerDesiredRotation: ControlRotation을 목표 회전값으로 삼아서 회전한다 [true]
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// RotationRate: 회전 속도
		GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

		// NPC 이동속도 너프
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}

	// 'OnFireEffect' 델리게이트에 OnFireEffect() 함수 바인드
	USAnimInstance* AnimInstance = Cast<USAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance) == true)
	{
		AnimInstance->OnFireEffect.AddDynamic(this, &ThisClass::OnFireEffect);
	}

	// 'BulletCount' Key 값 가져오기
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
			// WeaponMuzzleLocation: 무기 에셋의 'MuzzleFlash' 소켓 위치
			FVector WeaponMuzzleLocation = WeaponInstance->GetMesh()->GetSocketLocation(TEXT("MuzzleFlash"));
			// 플레이어 캐릭터의 머리 bone 위치
			FVector PlayerLocation = TargetPC->GetMesh()->GetSocketLocation(TEXT("head"));
			
			// TargetRotation: NPC 회전 목표치 설정
			FVector LookVector = PlayerLocation - WeaponMuzzleLocation;
			LookVector.Z = 0.f;
			FRotator TargetRotation = FRotationMatrix::MakeFromX(LookVector).Rotator();

			// NPC 부드럽게 회전
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.f));
		
			// 'CurrentAimPitch, CurrentAimYaw' 속성값 업데이트
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
	
	// NPC 죽음
	if (GetStatComponent()->GetCurrentHP() < KINDA_SMALL_NUMBER)
	{
		// NPC를 죽인 플레이어의 'CurrentKillCount' 값 증가
		ASPlayerCharacter* DamageCauserCharacter = Cast<ASPlayerCharacter>(DamageCauser);
		if (IsValid(DamageCauserCharacter) == true)
		{
			ASPlayerState* DCCPlayerState = Cast<ASPlayerState>(DamageCauserCharacter->GetPlayerState());
			if (IsValid(DCCPlayerState) == true)
			{
				DCCPlayerState->AddCurrentKillCount(1);
			}
		}
		
		// 연발 사격 타이머 클리어
		GetWorldTimerManager().ClearTimer(BetweenShotsTimer);

		// Behavior Tree 종료
		ASAIController* AIController = Cast<ASAIController>(GetController());
		if (true == ::IsValid(AIController))
		{
			AIController->EndAI();
		}

		// 죽는 애니메이션 재생 딜레이 후 파괴
		FTimerHandle deathTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(deathTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
			WeaponInstance->Destroy();
			WeaponInstance = nullptr;
		}), 1.2f, false);
	}
	// 피격 상태
	else {
		// 피격 애니메이션 재생
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
	// 캐릭터가 점프 중이거나 피격 중이면 공격 불가
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
			// 총알 장전 애니메이션 재생 중이면 return
			if (IsValid(AnimInstance) == false ||
				AnimInstance->Montage_IsPlaying(WeaponInstance->GetReloadAnimMontage()) == true) {
				return;
			}

			// 총알 유무 확인
			int32 BulletCount = AIController->GetBlackboardComponent()->GetValueAsInt(AIController->BulletCountKey);
			if (BulletCount == 0) {
				return;
			}
			// 총알 사용
			AIController->GetBlackboardComponent()->SetValueAsInt(AIController->BulletCountKey, BulletCount - 1);
#pragma endregion

#pragma region CaculateTargetTransform
			// WeaponMuzzleLocation: 무기 에셋의 'MuzzleFlash' 소켓 위치
			FVector WeaponMuzzleLocation = WeaponInstance->GetMesh()->GetSocketLocation(TEXT("MuzzleFlash"));
			// Tick() 함수에서 업데이트하는 'ControlRotation' 값
			FRotator ControlRotation = AIController->GetControlRotation();

			// TargetTransform: 사격 시작 지점
			FTransform TargetTransform = FTransform(ControlRotation, WeaponMuzzleLocation);
#pragma endregion

#pragma region LineTracing
		// BulletDirection: 사격 방향 -- TargetTransform의 Forward 벡터
		FVector BulletDirection = TargetTransform.GetUnitAxis(EAxis::X);
		// StartLocation: 사격 시작점 위치 (=FinalFocalLocation)
		FVector StartLocation = TargetTransform.GetLocation();
		// EndLocation: (사격 시작점 + 사거리) 위치
		FVector EndLocation = StartLocation + BulletDirection * WeaponInstance->GetMaxRange();

		// HitResult: 충돌 탐지 결과
		FHitResult HitResult;
		// TraceParams: 충돌 탐지 대상
		FCollisionQueryParams TraceParams(NAME_None, false, this);
		// 충돌 탐지 대상에서 무기 액터는 제외
		TraceParams.AddIgnoredActor(WeaponInstance);

		// LineTraceSingleByChannel: StartLocation에서 EndLocation 사이의 위치에서 TraceParams 중 'Attack' 채널이 적용되는 충돌 탐지 결과를 HitResult에 저장
		bool IsCollided = 
			GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel2, TraceParams);
		// 충돌 감지 실패 시 TraceStart, TraceEnd 값 직접 지정
		if (IsCollided == false)
		{
			HitResult.TraceStart = StartLocation;
			HitResult.TraceEnd = EndLocation;
		}

		// 사격 거리 디버그 드로잉
		DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Red, false, 0.5f, 0, 0.5f);
#pragma endregion

#pragma region DamageEvent
		if (IsCollided == true)
		{
			// 사격 적중한 플레이어 캐릭터에게 데미지 입히기
			ASPlayerCharacter* HittedCharacter = Cast<ASPlayerCharacter>(HitResult.GetActor());
			if (IsValid(HittedCharacter) == true)
			{
				FDamageEvent DamageEvent;

				// 사격 적중한 캐릭터에게 최종적으로 적용될 데미지
				float HitDamage = 10.f;

				// 헤드샷 적중 시 데미지 2배 적용
				FString BoneNameString = HitResult.BoneName.ToString();
				if (true == BoneNameString.Equals(FString(TEXT("HEAD")), ESearchCase::IgnoreCase))
				{
					HitDamage *= 2;
				}

				HittedCharacter->TakeDamage(HitDamage, DamageEvent, GetController(), this);
			}
		}

		// 사격 애니메이션 재생
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
		// WeaponSocket에서 총구까지의 거리 적용
		FVector ParticleLocation = FVector(-15.f, 65.f, 0.f);

		// 'FireParticleSystem' 효과 재생
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
	// 공격 애니메이션 재생 중이면 return
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false ||
		AnimInstance->Montage_IsPlaying(WeaponInstance->GetFireAnimMontage()) == true) {
		return;
	}

	// 총알 장전 애니메이션 재생
	if (IsValid(WeaponInstance->GetReloadAnimMontage()) == true)
	{
		AnimInstance->Montage_Play(WeaponInstance->GetReloadAnimMontage());
	}

	// 총알 개수 장전
	ASAIController* AIController = GetController<ASAIController>();
	if (IsValid(AIController) == true) {
		AIController->GetBlackboardComponent()->SetValueAsInt(AIController->BulletCountKey, MaxBulletCount);
	}
}

void ASNonPlayerCharacter::StartFire()
{
	// 연발 사격 타이머 세팅
	GetWorldTimerManager().SetTimer(BetweenShotsTimer, this, &ThisClass::TryFire, TimeBetweenFire, true);
}
