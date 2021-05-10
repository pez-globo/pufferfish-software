/**
 * @summary SVG based component for "Dropdown Arrow" icon
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 */
import SvgIcon, { SvgIconProps } from '@material-ui/core/SvgIcon';
import React from 'react';

/**
 * ArrowDropdown
 *
 * @component Component for displaying "Dropdown Arrow" icon
 *
 * @param {SvgIconProps} props - some desc for props
 *
 * @returns {JSX.Element} - "Dropdown Arrow" icon
 */
const ArrowDropdown = (props: SvgIconProps): JSX.Element => {
  return (
    <SvgIcon {...props} width="12.3" height="6.15" viewBox="0 0 12.3 6.15">
      <svg xmlns="http://www.w3.org/2000/svg">
        <g id="arrow-dropdown" opacity="0.984">
          <path
            id="Polygon_2"
            data-name="Polygon 2"
            d="M5.8.354a.5.5,0,0,1,.707,0L11.447,5.3a.5.5,0,0,1-.354.854H1.207A.5.5,0,0,1,.854,5.3Z"
            transform="translate(12.3 6.15) rotate(180)"
            fill="#fff"
          />
        </g>
      </svg>
    </SvgIcon>
  );
};

export default ArrowDropdown;
