module.exports = {
    "env": {
        "browser": true,
        "es6": true
    },
    "globals": {
        "Atomics": "readonly",
        "SharedArrayBuffer": "readonly"
    },
    "parser": "@typescript-eslint/parser",
    "parserOptions": {
        "ecmaFeatures": {
            "jsx": true
        },
        "ecmaVersion": 2020,
        "sourceType": "module"
    },
    "plugins": ["react", "@typescript-eslint", "prettier"],
    "extends": [
        "airbnb",
        "preact",
        "prettier",
        "prettier/react",
        "plugin:@typescript-eslint/recommended"
    ],
    "rules": {
        "import/extensions": [
            "error",
            "ignorePackages",
            {
                "js": "never",
                "jsx": "never",
                "ts": "never",
                "tsx": "never"
            }
        ],
        "import/no-unresolved": 0,
        "no-use-before-define": [
          "error",
          { "functions": true, "classes": true, "variables": false }
        ],
        
        "prettier/prettier": "error",
        "@typescript-eslint/explicit-function-return-type": "off",
        "@typescript-eslint/no-unused-vars": "off",
        "react/jsx-filename-extension": [
            1,
            { "extensions": [".js", ".jsx", ".ts", ".tsx"] }
        ]
    },
    "settings": {
        "import/resolver": {
            "node": {
                "extensions": [".js", ".jsx", ".ts", ".tsx"],
                "moduleDirectory": ['node_modules', 'src/'],
            }
        }
    }
}
