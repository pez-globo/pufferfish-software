// These types make it easier to write ES6 generators which yield results in
// addition to yielding redux-saga effects.

export enum GeneratorYieldType {
  Effect,
  Result,
}
export interface GeneratorYield<YieldResult, YieldEffect> {
  type: GeneratorYieldType;
  value: YieldResult | YieldEffect;
}

export const makeYieldResult = <YieldResult, YieldEffect>(
  result: YieldResult,
): GeneratorYield<YieldResult, YieldEffect> => ({
  type: GeneratorYieldType.Result,
  value: result,
});
export const makeYieldEffect = <YieldResult, YieldEffect>(
  effect: YieldEffect,
): GeneratorYield<YieldResult, YieldEffect> => ({
  type: GeneratorYieldType.Effect,
  value: effect,
});
