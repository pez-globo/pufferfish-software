import {createSelector} from 'reselect'
import enUS from '../../translations/en-us.json'
import {StoreState} from '../types'
import {LOCALE_DEFAULT, LOCALE_EN_US} from './types'

const MESSAGE_CATALOG: Record<string, Record<string, string>> = {
  [LOCALE_EN_US]: enUS,
}

export const getLocale = ({app}: StoreState): string => app.locale
export const getLocales = (): Array<string> => [LOCALE_EN_US]
export const getMessages = createSelector(
    getLocale, (locale: string): Record<string, string> => {
      return {
        ...MESSAGE_CATALOG[LOCALE_DEFAULT],
        ...MESSAGE_CATALOG[locale],
      }
    }
)
