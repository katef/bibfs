<?xml version="1.0"?>

<!--
	Copyright 2014-2017 Katherine Flavel

	See LICENCE for the full copyright terms.
-->

<xsl:stylesheet version="1.0"
	xmlns="http://www.w3.org/1999/xhtml"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:b="http://xml.elide.org/bibt">

	<xsl:output method="text"/>

	<xsl:template match="b:entry/@key">
		<xsl:value-of select="."/>
		<xsl:text>&#10;</xsl:text>
	</xsl:template>

	<xsl:template match="/b:bibt">
		<xsl:apply-templates select="b:entry/@key"/>
	</xsl:template>

</xsl:stylesheet>

