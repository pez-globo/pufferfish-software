/**
 * @summary Main entry point of the application
 *
 * @file Setting up theme, CSS styles & Routes are initialized here
 *
 */
import { CssBaseline } from '@material-ui/core';
import { MuiThemeProvider } from '@material-ui/core/styles';
import React, { Component } from 'react';
import { IntlProvider } from 'react-intl';
import { connect } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { initialize } from '../../store/app/actions';
import { getLocale, getMessages } from '../../store/app/selectors';
import { FrontendDisplaySetting, ThemeVariant } from '../../store/proto/frontend_pb';
import { getFrontendDisplaySetting } from '../../store/controller/selectors/frontend_pb';
import { StoreState } from '../../store/types';
import { darkTheme, lightTheme } from '../../styles/customTheme';
import Routes from './navigation/Routes';

const RIGHT_CLICK_BUTTON_KEY = 2;
const TOUCH_SCREEN_BUTTON_KEY = 0;

/**
 * @typedef Props
 *
 * Props interface for the App component
 *
 * @prop {typeof initialize} initialize Redux action type triggered on app initalization
 * @prop {string} locale Localization of the text displayed in app (i18n)
 * @prop {Record<string, string>} messages Localization configuration to set US English as locale
 * @prop {FrontendDisplaySetting} displaySetting Settings to set global config for theme & unit
 *
 */
interface Props {
  initialize: typeof initialize;
  locale: string;
  messages: Record<string, string>;
  displaySetting: FrontendDisplaySetting | null;
}

/**
 * App
 *
 * @component Component responsible to initalize app
 *
 * Uses the [[Props]] interface
 *
 */
class App extends Component<Props> {
  componentDidMount() {
    const { initialize } = this.props;
    initialize();

    /**
     * Disable Right click across app.
     *
     * @param {MouseEvent} event DOM mouse event object
     *
     * @return {boolean}
     *
     */
    const disableContext = (event: MouseEvent): boolean => {
      if (event.button === RIGHT_CLICK_BUTTON_KEY || event.button === TOUCH_SCREEN_BUTTON_KEY) {
        event.preventDefault();
        return false;
      }
      return true;
    };
    document.oncontextmenu = disableContext;
  }

  render() {
    const { locale, messages, displaySetting }: Props = this.props;
    const theme =
      displaySetting === null || displaySetting.theme === ThemeVariant.dark
        ? darkTheme
        : lightTheme;
    return (
      <IntlProvider key={locale} locale={locale} messages={messages}>
        <MuiThemeProvider theme={theme}>
          <CssBaseline />
          <Routes />
        </MuiThemeProvider>
      </IntlProvider>
    );
  }
}

/**
 * Selectors to access locale & displaySetting configuration from redux
 */
const selector = createStructuredSelector<
  StoreState,
  {
    locale: string;
    messages: Record<string, string>;
    displaySetting: FrontendDisplaySetting | null;
  }
>({
  locale: getLocale,
  messages: getMessages,
  displaySetting: getFrontendDisplaySetting,
});

const actions = {
  initialize,
};

export default connect(selector, actions)(App);
