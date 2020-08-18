import React from 'react'
import { makeStyles, Theme } from '@material-ui/core/styles'
import Grid from '@material-ui/core/Grid'
import { Typography } from '@material-ui/core'
import {
    RRInfo,
    PIPInfo,
    PEEPInfo,
    FiO2Info,
    IERatioInfo,
    TVInfo,
    InspTimeInfo,
    IPAbovePEEPInfo,
    PplatInfo,
    PawInfo,
    MVeInfo,
    EtCO2Info
} from '../utils/info'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        flexGrow: 1,
        justifyContent: 'center',
        alignItems: 'stretch',
        marginBottom: theme.spacing(2)
    },
    leftPanel: {
        border: '1px solid black',
        borderRadius: 16,
        marginRight: theme.spacing(2),
        backgroundColor: '#0b2e4c',
        color:'white'
    },
    rightContainer: {
        justifyContent: 'space-between',
        flexWrap: 'nowrap',
        width: '100%'
    },
    topRightPanel: {
        borderRadius: 16,
        border: '1px solid black',
        backgroundColor: '#0b2e4c',
        color:'white',
        width: '100%',
    },
    bottomRightPanel: {
        borderRadius: 16,
        //border: '1px solid black',
        backgroundColor: '#0b2e4c',
        color:'white',
        width: '100%',
        marginTop: theme.spacing(2)
    },
    bottomBorder: {
        borderBottom: '2px dashed black'
    },
    rightBorder: {
        borderRight: '2px dashed black'
    }
}))

/**
 * ValuesPage
 */
export const ValuesPage = () => {
    const classes = useStyles()

    return (
        <Grid container direction='row' className={classes.root}>
            <Grid container item xs direction='column' className={classes.leftPanel}>
                <Grid container item xs className={classes.bottomBorder}>
                    <Grid item xs className={classes.rightBorder}>
                        <RRInfo />
                    </Grid>
                    <Grid item xs>
                        <IPAbovePEEPInfo />
                    </Grid>
                </Grid>
                <Grid container item xs className={classes.bottomBorder}>
                    <Grid item xs className={classes.rightBorder}>
                        <PIPInfo />
                    </Grid>
                    <Grid item xs>
                        <InspTimeInfo />
                    </Grid>
                </Grid>
                <Grid container item xs>
                    <Grid item xs className={classes.rightBorder}>
                        <PEEPInfo />
                    </Grid>
                    <Grid item xs>
                        <FiO2Info />
                    </Grid>
                </Grid>
            </Grid>
            <Grid container item xs direction='column' className={classes.rightContainer} zeroMinWidth>
                <Grid container item xs direction='column' className={classes.topRightPanel}>
                    <Grid container item xs className={classes.bottomBorder}>
                        <Grid item xs className={classes.rightBorder}>
                            <PawInfo />
                        </Grid>
                        <Grid item xs>
                            <TVInfo />
                        </Grid>
                    </Grid>
                    <Grid container item xs>
                        <Grid item xs className={classes.rightBorder} >
                            <MVeInfo />
                        </Grid>
                        <Grid item xs>
                            <EtCO2Info />
                        </Grid>
                    </Grid>
                </Grid>
                <Grid item container className={classes.bottomRightPanel}>
                    <Grid item xs className={classes.rightBorder}>
                        <PplatInfo />
                    </Grid>
                    <Grid item xs>
                        <IERatioInfo />
                    </Grid>
                </Grid>
            </Grid>
        </Grid>
    )
}

export default ValuesPage