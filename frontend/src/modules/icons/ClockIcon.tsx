import React from 'react'
import SvgIcon, { SvgIconProps } from '@material-ui/core/SvgIcon'

const ClockIcon = (props: SvgIconProps) => {
    return (
        <SvgIcon {...props} width="87" height="24" viewBox="0 0 50 24">
            <svg xmlns="http://www.w3.org/2000/svg" width="87" height="24" viewBox="0 0 87 24">
                <g xmlns="http://www.w3.org/2000/svg" id="clock-icon" opacity="0.998">
                    <rect id="Box" width="24" height="24" fill="none" />
                    <path id="Union_2" data-name="Union 2" d="M0,10A10,10,0,1,1,9.99,20,9.994,9.994,0,0,1,0,10Zm2,0a8,8,0,1,0,8-8A8,8,0,0,0,2,10Zm7,1V5h1.5v5.25L15,12.92l-.751,1.229Z" transform="translate(2 2)" fill="#fff" />
                </g>
            </svg>
        </SvgIcon>
    )
}

export default ClockIcon