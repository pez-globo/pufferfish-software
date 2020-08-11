import { createMuiTheme, ThemeOptions } from '@material-ui/core/styles';

// Override Mui's theme typings to include custom theme properties.
declare module '@material-ui/core/styles/createMuiTheme' {
    interface Theme {
        panel: {
            borderRadius: React.CSSProperties['borderRadius'],
        }
    }
    interface ThemeOptions {
        panel?: {
            borderRadius?: React.CSSProperties['borderRadius']
        }
    }
}

function createCustomTheme(options: ThemeOptions) {
    return createMuiTheme({
        // Put custom theme properties below, but before `...options`.
        panel: {
            borderRadius: 16
        },
        // End custom attributes.
        ...options
    })
}

const darkTheme = createCustomTheme({
    palette: {
        type: 'dark',
        background: {
            paper: '#0b2e4c',
            default: 'black',
        },
        primary: {
            light: '#254764',
            main: '#0053b1',
        },
        secondary: {
            main: '#53d769',
        },
    },
    typography: {
        subtitle2: {
            fontSize: 12,
        },
        button: {
            textTransform: 'none'
        }
    },
    mixins: {
        toolbar: {
            minHeight: 80
        }
    },
})

export const customTheme = darkTheme