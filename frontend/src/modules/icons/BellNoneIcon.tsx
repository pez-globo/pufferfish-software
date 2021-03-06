/**
 * @summary A short one-line description for the file
 *
 */
import React from 'react';
import SvgIcon, { SvgIconProps } from '@material-ui/core/SvgIcon';

/**
 * BellNoneIcon
 *
 * @component Component for displaying "Bell" icon with blank filling
 *
 * @param {SvgIconProps} props - SVG props to override
 *
 * @returns {JSX.Element} - "Bell" with no filling icon used Sidebar for `Set Alarms` icon
 */
const BellNoneIcon = (props: SvgIconProps): JSX.Element => {
  return (
    <SvgIcon {...props} width="23.952" height="29.082" viewBox="0 0 23.952 29.082">
      <path
        id="Exclusion_2"
        data-name="Exclusion 2"
        d="M13334.489,8477.521h0a2.35,2.35,0,0,1-2.137-2.137h4.277A2.352,2.352,0,0,1,13334.489,8477.521Zm11.978-3.209h-23.952v-1.7c0-.5.289-.82.728-1.3,1.2-1.312,3.426-3.75,3.426-14.2,0-3.5,3.324-4.582,6.113-4.87v-2.877a.959.959,0,0,1,.92-.927l.055,0h1.467l.057,0a.962.962,0,0,1,.922.927v2.877c2.786.289,6.109,1.37,6.109,4.87,0,10.368,2.212,12.809,3.4,14.12.456.5.757.833.757,1.375v1.7Zm-11.968-20.435a7.235,7.235,0,0,0-1.319.08c-3.69.432-4.465,2.464-4.465,4.093,0,8.781-1.646,10.832-2.529,11.935a1.708,1.708,0,0,0-.539,1.1v1.434h17.686v-1.434a1.78,1.78,0,0,0-.559-1.157c-.877-1.1-2.51-3.153-2.51-11.873,0-2.945-2.453-3.852-4.512-4.093-.092-.01-.17-.02-.243-.029a5.439,5.439,0,0,0-.826-.053l-.15,0h-.033Z"
        transform="translate(-13322.515 -8448.438)"
      />
    </SvgIcon>
  );
};

export default BellNoneIcon;
