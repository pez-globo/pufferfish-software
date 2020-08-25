import React from 'react'
import { Button, Menu, MenuItem, makeStyles, Theme } from '@material-ui/core'
import { VentilationMode } from '../../store/controller/proto/mcu_pb'
import { useDispatch, useSelector } from 'react-redux'
import { getParametersRequestMode } from '../../store/controller/selectors'
import { updateCommittedParameter } from '../../store/controller/actions'
import { getModeText } from '../displays/ModeBanner'

const useStyles = makeStyles((theme: Theme) => ({
    button: {
        minWidth: 245
    },
}))

const modes = [
    VentilationMode.pc_ac,
    VentilationMode.pc_simv,
    VentilationMode.vc_ac,
    VentilationMode.vc_simv,
    VentilationMode.hfnc
]

/**
 * ModesDropdown
 * 
 * TODO: This component should rely on a redux state that stores the current
 *       mode as it has application-wide consequences. Updating that redux 
 *       state should be triggered by a dispatcher in the `handleItemClick` 
 *       handler.
 */
export const ModesDropdown = () => {
    const classes = useStyles()
    const [anchorEl, setAnchorEl] = React.useState<null | HTMLElement>(null)
    const dispatch = useDispatch()
    const ventilationMode = useSelector(getParametersRequestMode)
    const updateMode = (mode: VentilationMode) => dispatch(updateCommittedParameter({ mode: mode }))
    
    const handleClick = (event: React.MouseEvent<HTMLButtonElement>) => {
        setAnchorEl(event.currentTarget)
    }

    const handleItemClick = (
        _event: React.MouseEvent<HTMLElement, MouseEvent>,
        value: VentilationMode
    ) => {
        updateMode(value)
        handleClose()
    }

    const handleClose = () => {
        setAnchorEl(null)
    }

    return (
        <div>
            <Button onClick={handleClick} variant='contained' color='primary' className={classes.button}>
                {getModeText(ventilationMode)}
            </Button>
            <Menu
                id='simple-menu'
                anchorEl={anchorEl}
                keepMounted
                open={Boolean(anchorEl)}
                onClose={handleClose}
            >
                {modes.map((mode) => (
                    <MenuItem
                        key={mode}
                        selected={mode === ventilationMode}
                        onClick={(event) => handleItemClick(event, mode)}
                    >
                        {getModeText(mode)}
                    </MenuItem>
                ))}
            </Menu>
        </div>
    )
}

export default ModesDropdown