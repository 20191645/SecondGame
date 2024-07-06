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
	// Blackboard, BrainComponent: 'AIController' 클래스에 정의된 속성, 
	// UBrainComponent(부모) <- UBehaviorTreeComponent(자식)
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));
}

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	// 현재 빙의하고 있는 폰(NPC) 가져와서 AI 시작
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
		// 'BlackboardDataAsset'을 Blackboard로 사용
		if (UseBlackboard(BlackboardDataAsset, BlackboardComponent) == true)
		{
			// BehaviorTree 실행 및 확인
			bool bRunSucceeded = RunBehaviorTree(BehaviorTree);
			checkf(bRunSucceeded == true, TEXT("Fail to run behavior tree."));

			// 'BB_NPC'의 'StartPatrolPosition' Key에 현재 폰의 위치 저장
			BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());

			// 'BB_NPC'의 'BulletCount' Key에 최대 총알 개수 저장
			BlackboardComponent->SetValueAsInt(BulletCountKey, 20);
		}
	}
}

void ASAIController::EndAI()
{
	// BehaviorTree 실행 중단
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BehaviorTreeComponent) == true)
	{
		BehaviorTreeComponent->StopTree();
	}
}
