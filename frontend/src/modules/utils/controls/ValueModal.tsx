import React from 'react'
import ConfirmationModal from './ConfirmationModal'
import ValueController from './ValueController'

export interface SettingAdjustProps {
    committedSetting: number
}

interface Props {
    label: string,
    units: string,
    committedSetting: number,
    requestCommitSetting(setting: number): any,
}

export const ValueModal = (
    { label, units, committedSetting, requestCommitSetting }: Props
) => {
    const [value, setValue] = React.useState(committedSetting)

    return (
        <ConfirmationModal label='Set New' onConfirm={() => requestCommitSetting(value)}>
            <ValueController
                label={label}
                value={value}
                min={0} max={100}
                onClick={setValue}
                units={units}
            />
        </ConfirmationModal>
    )
}

export default ValueModal