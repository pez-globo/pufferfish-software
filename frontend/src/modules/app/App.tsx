import React, { Component } from 'react'
import { connect } from 'react-redux'
import { createStructuredSelector } from 'reselect'
import { IntlProvider } from 'react-intl'
import { CssBaseline } from '@material-ui/core'
import { MuiThemeProvider } from '@material-ui/core/styles'
import { customTheme } from '../../styles/customTheme'
import { StoreState } from '../../store/types'
import { initialize } from '../../store/app/actions'
import { getLocale, getMessages } from '../../store/app/selectors'
import Layout from './Layout'

interface Props {
  initialize: typeof initialize,
  locale: string,
  messages: Record<string, string>,
}

class App extends Component<Props> {
  componentDidMount() {
    const { initialize } = this.props
    initialize()
  }

  render() {
    const { locale, messages }: Props = this.props

    return (
      <IntlProvider key={locale} locale={locale} messages={messages}>
        <MuiThemeProvider theme={customTheme}>
          <CssBaseline />
          <Layout />
        </MuiThemeProvider>
      </IntlProvider>
    )
  }
}

const selector = createStructuredSelector<StoreState, { locale: string, messages: Record<string, string> }>({
  locale: getLocale,
  messages: getMessages,
})

const actions = {
  initialize,
}

export default connect(selector, actions)(App)