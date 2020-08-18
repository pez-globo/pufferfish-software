import { createMuiTheme, ThemeOptions } from '@material-ui/core/styles';

function createCustomTheme(options: ThemeOptions) {
    return createMuiTheme({
        /* Put custom style properties below, but before `...options`. */
        ...options
    })
}

export const customTheme = createCustomTheme({
    palette: {
        //type: 'dark',
    }
});
