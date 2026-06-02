// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	Intelligence.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	Intelligence.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	Intelligence.bSnapshot = false;
	
	RelevantAttributesToCapture.Add( Intelligence );
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 获取源和目标标签
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Intelligen =0.f;
	GetCapturedAttributeMagnitude(Intelligence,Spec,EvaluationParameters,Intelligen);
	Intelligen = FMath::Max<float>(Intelligen,0.f);
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	
	return 60.f + 2.5f * Intelligen + 10.f * PlayerLevel;
}
