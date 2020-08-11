import React from 'react'
import ValueSlider from './ValueSlider'
import ConfirmationModal from './ConfirmationModal'

export interface AlarmAdjustProps {
    committedMin: number,
    committedMax: number
}

interface Props {
    label: string,
    units: string,
    committedMin?: number,
    committedMax?: number,
    requestCommitRange(min: number, max: number): any
}

export const AlarmModal = (
    { label, committedMin = 0, committedMax = 100, requestCommitRange }: Props
) => {
    const [min] = React.useState(committedMin)
    const [max] = React.useState(committedMax)

    return (
        <ConfirmationModal label={'Alarm'} onConfirm={() => requestCommitRange(min, max)}>
            {/* TODO: Add Two controllers for setting the min and max limits of
                    the alarm's range 
            */}
            <ValueSlider label={label} min={min} max={max} />
        </ConfirmationModal>
    )
}

export default AlarmModal