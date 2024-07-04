// SAIController.h

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

class UBlackboardData;
class UBehaviorTree;

UCLASS()
class SECONDGAME_API ASAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASAIController();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	// AI 시작 함수
	void BeginAI(APawn* InPawn);
	// AI 종료 함수
	void EndAI();

public:
	// 순찰 범위
	static const float PatrolRadius;
	// 'BB_NPC'의 'StartPatrolPosition' Key 이름
	static const FName StartPatrolPositionKey;
	// 'BB_NPC'의 'EndPatrolPosition' Key 이름
	static const FName EndPatrolPositionKey;
	// 'BB_NPC'의 'TargetActor' Key 이름
	static const FName TargetActorKey;

private:
	// 'BB_NPC' 정보를 담을 속성
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;
	// 'BT_NPC' 정보를 담을 속성
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
