import React from 'react'
import { Link } from 'react-router-dom'
import Button from '@material-ui/core/Button';
import ArrowBackIcon from '@material-ui/icons/ArrowBack';

interface Props {
    label: string,
    path: string,
}

/**
 * BackButton 
 * 
 * A reusable back button for routing to previous page locations. 
 */
export const BackButton = ({ label, path }: Props) => {
    return (
        <Button
            component={Link}
            to={path}
            style={{ borderRadius: 8,
            background:'#0053b1',
            color:'white',
            paddingLeft:20,
            paddingRight:20 }}
            disableElevation
        >
            <ArrowBackIcon />
            {label}
        </Button>
    )
}

export default BackButton