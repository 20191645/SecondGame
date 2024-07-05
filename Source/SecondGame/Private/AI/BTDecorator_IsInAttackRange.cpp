// BTDecorator_IsInAttackRange.cpp

#include "AI/BTDecorator_IsInAttackRange.h"
#include "Controller/SAIController.h"
#include "Character/SNonPlayerCharacter.h"
#include "Character/SPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

const float UBTDecorator_IsInAttackRange::AttackRange(1200.f);

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	// Behavior Tree�� ���� ��� �̸� ����
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	checkf(bResult == true, TEXT("Super::CalculateRawConditionValue() function has returned false."));

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

	// �÷��̾� ĳ���Ͱ� ���� ���� ���� �ִ��� Ȯ��
	ASPlayerCharacter* TargetPlayerCharacter = 
		Cast<ASPlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ASAIController::TargetActorKey));
	if (IsValid(TargetPlayerCharacter) == true && TargetPlayerCharacter->IsPlayerControlled() == true)
	{
		bResult = NPC->GetDistanceTo(TargetPlayerCharacter) <= AttackRange;
		return bResult;
	}

	return false;
}
