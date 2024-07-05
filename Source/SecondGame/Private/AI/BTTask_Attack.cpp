// BTTask_Attack.cpp

#include "AI/BTTask_Attack.h"
#include "Controller/SAIController.h"
#include "Character/SNonPlayerCharacter.h"
#include "Animation/SAnimInstance.h"
#include "Item/SWeaponActor.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

	// NPC 사격 애니메이션 끝나면 반환
	UAnimInstance* AnimInstance = NPC->GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == true &&
		AnimInstance->Montage_IsPlaying(NPC->WeaponInstance->GetFireAnimMontage()) == false) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

	// NPC 연발 사격 함수 호출
	NPC->StartFire();

	return EBTNodeResult::InProgress;
}