/**
 * @summary SVG based component for "Gear" icon
 *
 */
import React from 'react';
import SvgIcon, { SvgIconProps } from '@material-ui/core/SvgIcon';

/**
 * GearIcon
 *
 * @component Component for displaying "Gear" icon
 *
 * @param {SvgIconProps} props - SVG props to override
 *
 * @returns {JSX.Element} - "Gear" icon used in Settings icon in navigation
 */
const GearIcon = (props: SvgIconProps): JSX.Element => {
  return (
    <SvgIcon {...props} width="28.451" height="28.451" viewBox="0 0 28.451 28.451">
      <path
        id="Path_108"
        data-name="Path 108"
        d="M23.65,9.247l1.956-3.734L23.117,3.023,19.382,4.979a6.37,6.37,0,0,0-1.956-.711L16,0H12.447L11.025,4.09A7.431,7.431,0,0,0,9.247,4.8L5.512,2.845,2.845,5.512,4.8,9.247a7.431,7.431,0,0,0-.711,1.778L0,12.447V16l4.09,1.423c.178.711.533,1.245.711,1.956L2.845,23.117l2.489,2.489L9.069,23.65a6.37,6.37,0,0,0,1.956.711l1.423,4.09H16l1.423-4.09c.711-.178,1.245-.533,1.956-.711l3.734,1.956,2.489-2.489L23.65,19.382a6.37,6.37,0,0,0,.711-1.956L28.451,16V12.447l-4.09-1.423A7.431,7.431,0,0,0,23.65,9.247ZM14.226,19.56a5.239,5.239,0,0,1-5.335-5.335,5.239,5.239,0,0,1,5.335-5.335,5.239,5.239,0,0,1,5.335,5.335A5.239,5.239,0,0,1,14.226,19.56Z"
      />
    </SvgIcon>
  );
};

export default GearIcon;
