import React from 'react'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { Grid, Typography, Button } from '@material-ui/core'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        justifyContent: 'space-between',
        alignItems: 'stretch',
        marginBottom: theme.spacing(2),
        color:'white'
    },
    modesPanel: {
        border: '1px solid black',
        borderRadius: 16,
        flexDirection: 'column',
        alignItems: 'stretch',
        marginRight: theme.spacing(2),
        backgroundColor: '#0b2e4c',
        color:'white'
    },
    modeContainer: {
        minHeight: 100,
        padding: theme.spacing(2),
        // border: '1px solid red',
    },
    modeTitleItem: {
        paddingTop: theme.spacing(2)
    },
    leftModeButtonOutline: {
        justifyContent: 'center',
        alignItems: 'center',
        marginRight: theme.spacing(2)
    },
    modeButtonOutline: {
        justifyContent: 'center',
        alignItems: 'center',
    },
    modeButton: {
        height: '60%',
        width: '100%',
        borderRadius: 16,
        color:'white',
        background:'#0053b1'
        //border: '2px solid black' 
    },
    abbrevPanel: {
        border: '1px solid black',
        flexDirection: 'column',
        borderRadius: 16,
        backgroundColor: '#0b2e4c',
    },
    abbrevContanier: {
        minHeight:64,
        padding: theme.spacing(2),
        // border: '1px solid red'
    },
    title: {
        borderBottom: '2px dashed black',
        minHeight: '60px',
        alignItems: 'center',
        padding: theme.spacing(2),
    }
}))

/**
 * ModesPage
 * 
 * TODO: This page should be hooked up into the redux store so that it has access
 *       to the current ventilation `mode` and a dispatcher to update the mode.
 */
export const ModesPage = () => {
    const classes = useStyles()

    return (
        <Grid container className={classes.root}>
            {/* Modes Panel  */}
            <Grid container item xs className={classes.modesPanel}>
                <Grid container item className={classes.title}>
                    <Typography variant='h3'>Modes</Typography>
                </Grid>
                <Grid container item xs>
                    <Grid container item direction='column' className={classes.modeContainer}>
                        <Grid item className={classes.modeTitleItem}>
                            <Typography variant='h6'>Pressure Controlled</Typography>
                        </Grid>
                        <Grid container item xs>
                            <Grid container item xs className={classes.leftModeButtonOutline}>
                                <Button className={classes.modeButton} variant='outlined'>
                                    <Typography variant='h5'>AC</Typography>
                                </Button>
                            </Grid>
                            <Grid container item xs className={classes.modeButtonOutline}>
                                <Button className={classes.modeButton} variant='outlined'>
                                    <Typography variant='h5'>SIMV</Typography>
                                </Button>
                            </Grid>
                        </Grid>
                    </Grid>
                    <Grid container item direction='column' className={classes.modeContainer}>
                        <Grid item className={classes.modeTitleItem}>
                            <Typography variant='h6'>Volume Controlled</Typography>
                        </Grid>
                        <Grid container item xs>
                            <Grid container item xs className={classes.leftModeButtonOutline}>
                                <Button className={classes.modeButton} variant='outlined'>
                                    <Typography variant='h5'>AC</Typography>
                                </Button>
                            </Grid>
                            <Grid container item xs className={classes.modeButtonOutline}>
                                <Button className={classes.modeButton} variant='outlined'>
                                    <Typography variant='h5'>SIMV</Typography>
                                </Button>
                            </Grid>
                        </Grid>
                    </Grid>
                    <Grid container item direction='column' className={classes.modeContainer}>
                        <Grid item className={classes.modeTitleItem}>
                            <Typography variant='h6'>Non-invasive</Typography>
                        </Grid>
                        <Grid container item xs>
                            <Grid container item xs className={classes.leftModeButtonOutline}>
                                <Button className={classes.modeButton} variant='outlined'>
                                    <Typography variant='h5'>NIV</Typography>
                                </Button>
                            </Grid>
                            <Grid container item xs>
                                {/* Blank Filler Space */}
                            </Grid>
                        </Grid>
                    </Grid>
                </Grid>
            </Grid>
            {/* Abbreviations Panel */}
            <Grid container item xs={5} className={classes.abbrevPanel}>
                <Grid container item className={classes.title}>
                    <Typography variant='h3'>Abbreviations</Typography>
                </Grid>
                <Grid container item alignItems='center' className={classes.abbrevContanier}>
                    <Grid item xs={2}>
                        <Typography><strong>AC</strong>:</Typography>
                    </Grid>
                    <Grid item xs>
                        <Typography>Assisted Control</Typography>
                    </Grid>
                </Grid>
                <Grid container item alignItems='center'className={classes.abbrevContanier}>
                    <Grid item xs={2}>
                        <Typography><strong>SIMV</strong>:</Typography>
                    </Grid>
                    <Grid item xs>
                        <Typography>Synchronized Intermittent Mandatory Ventilation</Typography>
                    </Grid>
                </Grid>
                <Grid container item alignItems='center' className={classes.abbrevContanier}>
                    <Grid item xs={2}>
                        <Typography><strong>NIV</strong>:</Typography>
                    </Grid>
                    <Grid item xs>
                        <Typography>Non-invasive Ventilation</Typography>
                    </Grid>
                </Grid>
            </Grid>
        </Grid >
    )
}

export default ModesPage