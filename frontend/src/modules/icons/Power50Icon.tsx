import React from 'react';
import SvgIcon, { SvgIconProps } from '@material-ui/core/SvgIcon';
import { makeStyles, Theme } from '@material-ui/core/styles';

const useStyles = makeStyles((theme: Theme) => ({
  path: {
    color: theme.palette.text.primary,
  },
}));

const Power50Icon = (props: SvgIconProps): JSX.Element => {
  const classes = useStyles();
  return (
    <SvgIcon {...props} width="73" height="19" viewBox="0 0 50 19">
      <g id="Battery_25_-_50_" data-name="Battery (25 - 50)" transform="translate(-909 -35)">
        <g id="Group_1564" data-name="Group 1564" transform="translate(-44)">
          <g id="Group_1563" data-name="Group 1563" transform="translate(21817.146 19909.445)">
            <path
              id="Exclusion_3"
              data-name="Exclusion 3"
              d="M24,16H3a3,3,0,0,1-3-3V3A3,3,0,0,1,3,0H24a3,3,0,0,1,3,3V13A3,3,0,0,1,24,16ZM3.495,1.428a2,2,0,0,0-2,2v9.142a2,2,0,0,0,2,2H23.6a2,2,0,0,0,2-2V3.429a2,2,0,0,0-2-2Z"
              transform="translate(-20864.146 -19874.445)"
              className={classes.path}
            />
            <rect
              id="Rectangle_1738"
              data-name="Rectangle 1738"
              width="11"
              height="10"
              rx="1"
              transform="translate(-20861.146 -19871.445)"
              className={classes.path}
            />
            <path
              id="Rectangle_1739"
              data-name="Rectangle 1739"
              d="M0,0H1A2,2,0,0,1,3,2V4A2,2,0,0,1,1,6H0A0,0,0,0,1,0,6V0A0,0,0,0,1,0,0Z"
              transform="translate(-20836.146 -19869.445)"
              className={classes.path}
            />
          </g>
        </g>
      </g>
    </SvgIcon>
  );
};

export default Power50Icon;
