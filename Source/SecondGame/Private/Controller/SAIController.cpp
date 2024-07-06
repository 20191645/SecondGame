// SAIController.cpp

#include "Controller/SAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

const float ASAIController::PatrolRadius(1500.f);
const FName ASAIController::StartPatrolPositionKey(TEXT("StartPatrolPosition"));
const FName ASAIController::EndPatrolPositionKey(TEXT("EndPatrolPosition"));
const FName ASAIController::TargetActorKey(TEXT("TargetActor"));
const FName ASAIController::BulletCountKey(TEXT("BulletCount"));

ASAIController::ASAIController()
{
	// Blackboard, BrainComponent: 'AIController' Ŭ������ ���ǵ� �Ӽ�, 
	// UBrainComponent(�θ�) <- UBehaviorTreeComponent(�ڽ�)
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));
}

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	// ���� �����ϰ� �ִ� ��(NPC) �����ͼ� AI ����
	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn) == true)
	{
		BeginAI(ControlledPawn);
	}
}

void ASAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndAI();

	Super::EndPlay(EndPlayReason);
}

void ASAIController::BeginAI(APawn* InPawn)
{
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
	if (IsValid(BlackboardComponent) == true)
	{
		// 'BlackboardDataAsset'�� Blackboard�� ���
		if (UseBlackboard(BlackboardDataAsset, BlackboardComponent) == true)
		{
			// BehaviorTree ���� �� Ȯ��
			bool bRunSucceeded = RunBehaviorTree(BehaviorTree);
			checkf(bRunSucceeded == true, TEXT("Fail to run behavior tree."));

			// 'BB_NPC'�� 'StartPatrolPosition' Key�� ���� ���� ��ġ ����
			BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());

			// 'BB_NPC'�� 'BulletCount' Key�� �ִ� �Ѿ� ���� ����
			BlackboardComponent->SetValueAsInt(BulletCountKey, 20);
		}
	}
}

void ASAIController::EndAI()
{
	// BehaviorTree ���� �ߴ�
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BehaviorTreeComponent) == true)
	{
		BehaviorTreeComponent->StopTree();
	}
}
