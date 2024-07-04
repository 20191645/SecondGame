// BTTask_GetEndPatrolPosition.cpp

#include "AI/BTTask_GetEndPatrolPosition.h"
#include "Controller/SAIController.h"
#include "Character/SNonPlayerCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetEndPatrolPosition::UBTTask_GetEndPatrolPosition()
{
	// Behavior Tree에 보일 노드 이름 설정
	NodeName = TEXT("GetEndPatrolPosition");
}

EBTNodeResult::Type UBTTask_GetEndPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}

	ASAIController* AIController = Cast<ASAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController"));

	ASNonPlayerCharacter* NPC = Cast<ASNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC"));

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(NPC->GetWorld());
	checkf(IsValid(NavigationSystem) == true, TEXT("Invalid NavigationSystem"));

	// NPC의 현재 위치 가져오기
	FVector StartPatrolPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ASAIController::StartPatrolPositionKey);
	FNavLocation EndPatrolLocation;
	// 순찰 범위 내 랜덤한 위치 가져오기
	if (true == NavigationSystem->GetRandomPointInNavigableRadius(StartPatrolPosition, AIController->PatrolRadius, EndPatrolLocation))
	{
		// NPC의 목표 위치 업데이트
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ASAIController::EndPatrolPositionKey, EndPatrolLocation.Location);
		return Result = EBTNodeResult::Succeeded;
	}

	return Result;
}
