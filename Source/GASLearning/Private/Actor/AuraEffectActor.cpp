// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

/**
假设你（AAuraEffectActor）是一个 摄像头，目标（Target）是一辆 超速的汽车。
GetAbilitySystemComponent: 你得先确认这辆车有没有 “罚单接收箱”（ASC）。如果是辆自行车（没有 ASC），你没法贴罚单。
MakeEffectContext: 你准备写罚单了。你需要记录 “是谁拍的照片”（也就是 this，摄像头自己）。
MakeOutgoingSpec: 你手里有一本 “空白罚单本”（GE Class），你撕下来一张，填好日期、等级（1.0f），这就变成了一张 “生效的罚单”（Spec）。
ApplyGameplayEffectSpecToSelf: 你把这张填好的罚单，贴 到了汽车的脑门上。
 */
void AAuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC == nullptr) return;
	
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle =TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle =TargetASC->MakeOutgoingSpec(GameplayEffectClass,1.f,EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

