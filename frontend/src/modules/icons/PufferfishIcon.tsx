/**
 * @summary SVG based component for "Pufferfish" icon
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 */
import React from 'react';
import SvgIcon, { SvgIconProps } from '@material-ui/core/SvgIcon';

/**
 * PufferfishIcon
 *
 * @component Component for displaying "Pufferfish" icon
 *
 * @param {SvgIconProps} props - SVG props to override
 *
 * @returns {JSX.Element} - "Pufferfish" icon used in Screen saver page
 */
const PufferfishIcon = (props: SvgIconProps): JSX.Element => {
  return (
    <SvgIcon {...props} width="30.528" height="24.391" viewBox="0 0 30.528 24.391">
      <g id="Group_986" data-name="Group 986" transform="translate(0 0.001)">
        <g id="Group_787" data-name="Group 787" transform="translate(5.144 -0.001)">
          <g id="Group_786" data-name="Group 786">
            <g id="Group_784" data-name="Group 784">
              <path
                id="Path_1"
                data-name="Path 1"
                d="M517.95,384.387c-.306-1.177-.248,5.268.279,3.559.141-.455.1-.006.256-.347.009-.019.174-.411.193-.462s.658-1.76.691-1.844a16.561,16.561,0,0,1,3.563-5.857,10.711,10.711,0,0,1,18.673,8.015,10.14,10.14,0,0,1-.6,2.713,8.572,8.572,0,0,1-1,2.036,11.442,11.442,0,0,1-1.659,2.045,10.782,10.782,0,0,1-15.65-.709c-1.191-1.538.008.051-1.926-2.469-.451-.591-.9-1.047-1.407-.708-.419.288-.276.8.235,1.516a21.414,21.414,0,0,0,4.036,4.4,12.159,12.159,0,0,0,16.079-1.3,11.561,11.561,0,0,0,1.719-2.2,7.325,7.325,0,0,0,.964-2.091A11.838,11.838,0,0,0,539.71,378.1a11.639,11.639,0,0,0-10.308-3.677,11.811,11.811,0,0,0-7.551,3.979,25.087,25.087,0,0,0-2.716,3.643A22.832,22.832,0,0,0,517.95,384.387Z"
                transform="translate(-517.764 -374.327)"
              />
            </g>
          </g>
        </g>
        <g id="Group_789" data-name="Group 789" transform="translate(0 4.731)">
          <path
            id="Exclusion_1"
            data-name="Exclusion 1"
            d="M1.942,12.15h0a1.268,1.268,0,0,1-.834-.3,4.953,4.953,0,0,1-.99-2.958,22.418,22.418,0,0,1,0-4.6C.351,2.015.882.447,1.538.1A.789.789,0,0,1,1.915,0,1.826,1.826,0,0,1,3.122.7C4.7,2.346,6.189,6.281,6.13,7.544a5.658,5.658,0,0,1-1.466,2.877A4.2,4.2,0,0,1,1.942,12.15Zm.374-9.8c-.734,0-1.13,2.106-1.13,4.088,0,2,.388,4.032,1.13,4.032S4.585,8.74,4.585,6.735C4.585,4.761,3.066,2.348,2.316,2.348Z"
            transform="translate(0 0)"
          />
        </g>
      </g>
    </SvgIcon>
  );
};

export default PufferfishIcon;
