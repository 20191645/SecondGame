// BTTask_Reload.cpp

#include "AI/BTTask_Reload.h"
#include "Controller/SAIController.h"
#include "Character/SNonPlayerCharacter.h"
#include "Animation/SAnimInstance.h"
#include "Item/SWeaponActor.h"

UBTTask_Reload::UBTTask_Reload()
{
	bNotifyTick = true;
}

void UBTTask_Reload::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

	// NPC �Ѿ� ���� �ִϸ��̼� ������ ��ȯ
	UAnimInstance* AnimInstance = NPC->GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == true &&
		AnimInstance->Montage_IsPlaying(NPC->WeaponInstance->GetReloadAnimMontage()) == false) {
		// 1�� ������
		FTimerHandle reloadTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(reloadTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}), 1.0f, false);
	}
}

EBTNodeResult::Type UBTTask_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

	// NPC �Ѿ� ���� �Լ� ȣ��
	NPC->Reload();

	return EBTNodeResult::InProgress;
}
