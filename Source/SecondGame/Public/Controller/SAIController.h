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
	// AI ���� �Լ�
	void BeginAI(APawn* InPawn);
	// AI ���� �Լ�
	void EndAI();

public:
	// ���� ����
	static const float PatrolRadius;
	// 'BB_NPC'�� 'StartPatrolPosition' Key �̸�
	static const FName StartPatrolPositionKey;
	// 'BB_NPC'�� 'EndPatrolPosition' Key �̸�
	static const FName EndPatrolPositionKey;
	// 'BB_NPC'�� 'TargetActor' Key �̸�
	static const FName TargetActorKey;

private:
	// 'BB_NPC' ������ ���� �Ӽ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;
	// 'BT_NPC' ������ ���� �Ӽ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
